#include "ProcessImages.h"

void ProcessImages::initLabels() {
    metric.readLabels();
    metric.setTotalAmountLicensePlatesOnPictures();
    metric.setTotalAmountOfCharactersOnPictures();
    metric.printLabels();
}

void ProcessImages::preprocessImage(cv::Mat image) {
    cv::erode(image, erodeResult, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2)));
    cv::dilate(erodeResult, dilateResult, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2)));

    //cv::dilate(image, dilateResult, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));
    //cv::erode(dilateResult, erodeResult, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));

    //cv::resize(dilateResult, resizedDown, cv::Size(down_width, down_height), cv::INTER_LINEAR);

    cv::cvtColor(dilateResult, imageGrey, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(imageGrey, imageBlurred, cv::Size(3, 3), 0);

    cv::threshold(imageBlurred, imageThresh, 129, 255, cv::THRESH_BINARY);

    //cv::Sobel(imageThresh, sobel_x, ddepth, 1, 0, 3, cv::BORDER_DEFAULT);
    //cv::Sobel(imageThresh, sobel_y, ddepth, 0, 1, 3, cv::BORDER_DEFAULT);
    //cv::magnitude(sobel_x, sobel_y, sobelResult);

    //cv::hconcat(image, imageThresh, imageConcat);
}

void ProcessImages::execute() {
    getDesktopResolution();
    process();
    metric.printRecognizedOutOfAll();
    cv::destroyAllWindows();
}

void ProcessImages::process() {
    for (int i = 1; i < NUMBER_OF_IMAGES + 1; i++) {
        index = i - 1;
        std::string imageToRead = "images/" + std::to_string(i) + ".png";

        image = cv::imread(imageToRead);

        if (image.empty()) {
            imageToRead = "images/" + std::to_string(i) + ".jpg";
            image = cv::imread(imageToRead);

            if (image.empty()) {
                std::cout << "Image not Found !!!" << std::endl;
                std::cin.get();
                continue;
            }
        }

        filterImageData();
        printFilterConditions(i);

        if (isImageBright(image)) {
            //std::cout << "Image is Bright" << std::endl;
            preprocessImage(image);
            filterContours(true);
        }
        else {
            //std::cout << "Image is not Bright ! " << std::endl;
            makeBrighter(2.0, 10);
            gammaCorrection(image, 0.5);
            preprocessImage(res);
            preprocessImage(newImage);
            filterContours(false);
        }
    }
}

void ProcessImages::printFilterConditions(int i)
{
    std::cout << "[Filter Conditions for Picture " << i << "]\n";
    std::cout << "Min Area outer Contour : " << minAreaOuter << " --- " << "Max Area outer Contour : " << maxAreaOuter << "\n";
    std::cout << "White Pixel Threshold lower boundary for rectangle : " << WHITE_PIXEL_TRESHOLD_LOW << "\n";
    std::cout << "White Pixel Threshold upper boundary for rectangle : " << WHITE_PIXEL_THRESHOLD_HIGH << "\n";
    std::cout << "Min aspect ratio for rect : " << ASPECT_RATIO_MIN << " --- " << "Max aspect ratio for rect : " << ASPECT_RATIO_MAX << std::endl;
    std::cout << "Darkness Threshold for whole picture : " << DARKNESS_THRESHOLD << "\n\n";
}

void ProcessImages::filterImageData()
{
    metric.labels.at(index).width = image.rows;
    metric.labels.at(index).heigth = image.cols;

    minAreaOuter = (image.rows / 50.0f) * (image.cols / 50.0f);
    maxAreaOuter = (image.rows / 8.0f) * (image.cols / 8.0f);
}

void ProcessImages::filterContours(bool brighter) {
    float proportionAreaWhitepixels = 0.f;
    cv::Mat image_copy;
    std::vector<cv::Point> approx;
    double perimeter = 0.0;
  
    if (brighter) {
        image_copy = image.clone();
    }
    else {
        image_copy = newImage.clone();
    }
    
    //Draw optimal
    for (int h = 0; h < metric.labels.at(index).coordinates.size(); h++) {
        cv::rectangle(image_copy, cv::Rect(metric.labels.at(index).coordinates.at(h).x, metric.labels.at(index).coordinates.at(h).y, 50, 50), cv::Scalar(0, 0, 255), 2);
    }

    cv::findContours(imageThresh, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE); // What influence do RETR_TREE and CHAIN_APPROX_NONE have on the algorithm
    
    metric.recognizedPerPicture = 0;
    metric.falsePositiveCounterPerPicture = 0;
    std::vector<int> arrayRecognizedCharacters;

    for (int i = 0; i < contours.size(); i++) {
        perimeter = cv::arcLength(contours.at(i), true);
        cv::approxPolyDP(contours.at(i), approx, 0.04 * perimeter, true);

        if (approx.size() == 4) {
            cv::Rect2f resultRect = cv::boundingRect(approx);

            if (!isRectangle(resultRect)) {
                //std::cout << "Eliminated not recognized as rectangle ! Area or Ratio does not fit" << std::endl;
                continue;
            }

            cv::Mat subImg = imageThresh(resultRect);
            double whitePixelsInRectangle = cv::countNonZero(subImg);
            proportionAreaWhitepixels = (whitePixelsInRectangle / (resultRect.area()));
            printCandidateData(i, resultRect, whitePixelsInRectangle, proportionAreaWhitepixels);
            
            if (proportionAreaWhitepixels < WHITE_PIXEL_TRESHOLD_LOW || proportionAreaWhitepixels > WHITE_PIXEL_THRESHOLD_HIGH) {
                std::cout << "Eliminated to high or to low white pixel ratio !" << "\n\n";
                continue;
            }
            
            cv::findContours(subImg, contoursSecond, hierarchySecond, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
            cv::Mat subImgCopy = image_copy(resultRect);
            cv::drawContours(subImgCopy, contoursSecond, -1, cv::Scalar(0, 255, 255), 1, cv::LINE_AA);

            setFilterConditionsInnerContours(subImgCopy);

            unsigned int contourCounter = 0;
            countInnerContours(contourCounter, i, subImgCopy);

            if (contourCounter < 4 || contourCounter > 12) {
                std::cout << "Eliminated to many or to little contours counted !" << "\n\n";
                continue;
            }
            std::cout << "\n[Candidate " << std::to_string(i) + "]" << " Characters on license plate: " << contourCounter - 1 << "\n\n";
            cv::rectangle(image_copy, resultRect, cv::Scalar(0, 255, 0), 2);
            
            bool isFalsePositive = false;

            for (int h = 0; h < metric.labels.at(index).coordinates.size(); h++) {
                float d = distanceBetweenTwoPoints(resultRect.x, resultRect.y, metric.labels.at(index).coordinates.at(h).x, metric.labels.at(index).coordinates.at(h).y);
                std::cout << "Distance : " << d << std::endl;

                if (d > FALSE_POSITIVE_THRESHOLD) {
                    isFalsePositive = true;
                }
                if (d < FALSE_POSITIVE_THRESHOLD) {
                    isFalsePositive = false;
                    break;
                }
            }
            
            if (isFalsePositive) {
                metric.falsePositiveCounterPerPicture++;
                contourCounter = 0;
                break;
            }

            arrayRecognizedCharacters.push_back(contourCounter);
            metric.recognizedCharacters += contourCounter;
            metric.recognizedLicensePlates++;
            metric.recognizedPerPicture++;

            if (metric.recognizedPerPicture > metric.labels.at(index).numberplateCount) {
                metric.recognizedLicensePlates -= metric.recognizedPerPicture;
                metric.recognizedCharacters -= contourCounter;
                break;
            }
        }
    }
    metric.recognizedCharactersPerLicenseplate.push_back(arrayRecognizedCharacters);
    metric.printMetricOnePicture(index);
    if (DEBUG_FLAG_SHOW_RESULT) {
        displayResults(image_copy);
        cv::destroyAllWindows();
    }
}

void ProcessImages::setFilterConditionsInnerContours(cv::Mat& subImgCopy)
{
    minAreaInner = (subImgCopy.rows / 6.0f) * (subImgCopy.cols / 6.0f);
    maxAreaInner = (subImgCopy.rows / 3.2f) * (subImgCopy.cols / 3.2f);
    std::cout << "Min Area inner Contour : " << minAreaInner << " --- " << "Max Area inner Contour : " << maxAreaInner << "\n";
}

void ProcessImages::displayResults(cv::Mat& image_copy)
{
    float resizeFactor = 0.6;
    if (image_copy.rows > 3000.0f || image_copy.cols > 3000.0f) {
        resizeFactor = 0.25;
    }
    cv::resize(image_copy, resizedDown, cv::Size(image_copy.rows * resizeFactor, image_copy.cols * resizeFactor), cv::INTER_LINEAR);
    cv::resize(imageThresh, imageThreshResized, cv::Size(imageThresh.rows * resizeFactor, imageThresh.cols * resizeFactor), cv::INTER_LINEAR);
    cv::namedWindow("Result");
    cv::namedWindow("Thresh");
    cv::moveWindow("Result", 0, 0);
    cv::moveWindow("Thresh", desktopWidth / 2, 0);
    cv::imshow("Result", resizedDown);
    cv::imshow("Thresh", imageThreshResized);
    cv::waitKey(0);
}

void ProcessImages::countInnerContours(unsigned int& contourCounter, int i, cv::Mat& subImgCopy)
{
    for (int j = 0; j < contoursSecond.size(); j++) {
        double perimeter = cv::arcLength(contoursSecond.at(j), true);
        std::vector<cv::Point> approx;
        cv::approxPolyDP(contoursSecond.at(j), approx, 0.04 * perimeter, true);
        cv::Rect2f resultRect = cv::boundingRect(approx);

        if(resultRect.area() < minAreaInner || resultRect.area() > maxAreaInner) {
            //std::cout << "Eliminated area of inner Contour does not match min or max !" << std::endl;
            continue;
        }
        contourCounter++;
        std::cout << "[Candidate " + std::to_string(i) + "]" << " Area of Contour : " << resultRect.area() << " --- " << "Perimeter : " << perimeter << "ContourCounter : " << contourCounter << std::endl;
        cv::Mat character = subImgCopy(resultRect);
    }
}

void ProcessImages::printCandidateData(int i, cv::Rect2f& resultRect, double whitePixelsInRectangle, float proportionAreaWhitepixels)
{
    std::cout << "[Candidate " + std::to_string(i) + "]" << " Area: " << resultRect.area() << std::endl;
    std::cout << "[Candidate " + std::to_string(i) + "]" << " Number of white pixels: " << whitePixelsInRectangle << std::endl;
    std::cout << "[Candidate " + std::to_string(i) + "]" << " Ratio white pixel to area of rectangle : " << proportionAreaWhitepixels << "\n";

}

bool ProcessImages::isRectangle(cv::Rect2f& resultRect)
{
    float aspectratio = resultRect.width / resultRect.height;
    if (aspectratio < 1.0f) {
        aspectratio = 1 / aspectratio;
    }

    if ((aspectratio < ASPECT_RATIO_MIN || aspectratio > ASPECT_RATIO_MAX) || (resultRect.area() < minAreaOuter || resultRect.area() > maxAreaOuter)){
        return false;
    }
    return true;
}

bool ProcessImages::isImageBright(cv::Mat image) {
    cv::Mat hsv;
    cv::Mat copy = image.clone();
    cv::cvtColor(copy, hsv, cv::COLOR_BGR2HSV);

    const auto result = cv::mean(hsv);

    metric.labels.at(index).brightness = result[2];

    if (result[2] < DARKNESS_THRESHOLD) {
        return 0;
    }
    else {
        return 1;
    }

}

void ProcessImages::makeBrighter(double alpha, int beta) {
    newImage = cv::Mat::zeros(image.size(), image.type());

    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            for (int c = 0; c < image.channels(); c++) {
                newImage.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(alpha * image.at<cv::Vec3b>(y, x)[c] + beta);
            }
        }
    }

    //cv::imshow("Original Image", image);
    //cv::imshow("New Image", new_image);

    //cv::waitKey();
}

void ProcessImages::gammaCorrection(const cv::Mat& img, const double gamma_) {
    CV_Assert(gamma_ >= 0);

    cv::Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();

    for (int i = 0; i < 256; ++i) {
        p[i] = cv::saturate_cast<uchar>(cv::pow(i / 255.0, gamma_) * 255.0);
    }

    res = img.clone();
    cv::LUT(img, lookUpTable, res);
    // cv::imshow("Gamma correction", res);
}

void ProcessImages::getDesktopResolution() {
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    desktopWidth = desktop.right;
    desktopHeigth = desktop.bottom;
}

float ProcessImages::distanceBetweenTwoPoints(float x1, float y1, float x2, float y2){
    float diffX = x2 - x1;
    float diffY = y2 - y1;

    return sqrtf(pow(diffX, 2.0f) + pow(diffY, 2.0f));
}