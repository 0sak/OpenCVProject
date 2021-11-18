#include "ProcessImages.h"

void ProcessImages::processImage(cv::Mat image) {

    cv::erode(image, erodeResult, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
    cv::dilate(erodeResult, dilateResult, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

    //cv::resize(dilateResult, resizedDown, cv::Size(down_width, down_height), cv::INTER_LINEAR);

    cv::cvtColor(dilateResult, imageGrey, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(imageGrey, imageBlurred, cv::Size(3, 3), 0);

    cv::threshold(imageBlurred, imageThresh, 127, 255, cv::THRESH_BINARY);

    //cv::Sobel(imageThresh, sobel_x, ddepth, 1, 0, 3, cv::BORDER_DEFAULT);
    //cv::Sobel(imageThresh, sobel_y, ddepth, 0, 1, 3, cv::BORDER_DEFAULT);
    //cv::magnitude(sobel_x, sobel_y, sobelResult);

    //cv::hconcat(imageGrey, sobelResult, imageConcat);

}

void ProcessImages::processAll() {
    for (int i = 1; i < NUMBER_OF_IMAGES + 1; i++) {

        std::string imageToRead = "images/";
        imageToRead += std::to_string(i);
        imageToRead += ".png";

        image = cv::imread(imageToRead);

        if (image.empty()) {
            std::cout << "Image not Found !!!" << std::endl;
            std::cin.get();
        }

        processImage(image);

        cv::imshow(imageToRead, imageThresh);
        getContours();

        cv::waitKey(0);
    }
    cv::destroyAllWindows();
}

void ProcessImages::getContours() {
    cv::findContours(imageThresh, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

    cv::Mat image_copy = image.clone();

    for (int i = 0; i < contours.size(); i++) {
        double perimeter = cv::arcLength(contours.at(i), true);
        std::vector<cv::Point> approx;
        cv::approxPolyDP(contours.at(i), approx, 0.04 * perimeter, true);
        if (approx.size() == 4) {
            //std::cout << "Found :)" << std::endl;
            cv::Rect2f resultRect = cv::boundingRect(approx);

            float aspectratio = resultRect.width / resultRect.height;
            if (aspectratio < 1.0f){
                aspectratio = 1 / aspectratio;
            }

            float aspect = 4.7272;

            float min = 20 * aspect * 15;
            float max = 200 * aspect * 200;

            float rmin = 3;
            float rmax = 4.9;

            if ((aspectratio < rmin || aspectratio > rmax)  || (resultRect.area() < min || resultRect.area() > max)) {
                continue;
            }


            cv::rectangle(image_copy, resultRect, cv::Scalar(0, 255, 0), 2);
            //cv::drawContours(image_copy, contours.at(i), -1, cv::Scalar(0, 255, 0), 2);
        }
    }

    cv::imshow("None appoxiation", image_copy);
    cv::waitKey(0);
    
}