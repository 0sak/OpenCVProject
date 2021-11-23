#include "ProcessImages.h"


/*
* Nimmt als Input das eingelsene Bild, auf dem die Nummernschilder erkannt werden sollen.
* Funktion führt Preprocessing durch, bestehend aus Opening, Grayscaling, Blurring, Thresholding.
*/

void ProcessImages::processImage(cv::Mat image) {

    cv::erode(image, erodeResult, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
    cv::dilate(erodeResult, dilateResult, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

    //cv::resize(dilateResult, resizedDown, cv::Size(down_width, down_height), cv::INTER_LINEAR);

    cv::cvtColor(dilateResult, imageGrey, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(imageGrey, imageBlurred, cv::Size(3, 3), 0);

    cv::threshold(imageBlurred, imageThresh, 155, 255, cv::THRESH_BINARY);

    //cv::Sobel(imageThresh, sobel_x, ddepth, 1, 0, 3, cv::BORDER_DEFAULT);
    //cv::Sobel(imageThresh, sobel_y, ddepth, 0, 1, 3, cv::BORDER_DEFAULT);
    //cv::magnitude(sobel_x, sobel_y, sobelResult);

    //cv::hconcat(image, imageThresh, imageConcat);

}
 /*
 * Funktion die in der Main aufgerufen wird, um das Vorverarbeiten und das Filtern der Konturen einzuleiten, liest auch Bild nach Bild ein.
 * Quasi unser GameLoop :d
 */
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

        //cv::imshow(imageToRead, imageThresh);
        getContours();

        //cv::waitKey(0);
    }
    cv::destroyAllWindows();
}

/*
* Nimmt sich das Bild mit dem Threshold Filter und erkennt darauf die Konturen, der Umfang der Kontur wird erkannt, die Kontur wird Approximiert,
* es wird also entschieden aus wie vielen Vertices (Punkten) diese Kontur besteht, ein Rechteck/Quadrat besteht beispielsweise aus 4.
* Wenn die Kontur tatsächlich aus 4 Punkten besteht wird das Rechteck in gespeichert in resultRect, dann werden Seitenverhätlnis und Fläche 
* geprüft. Wenn die Fläche nicht zu klein oder zu groß ist und das Seitenverhältnis stimmt wird das gespeicherte Rechteck gezeichnet,
* ansonsten wird übersprungen und die nächste Kontur wird analysiert.
*/
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
    cv::hconcat(imageBlurred, imageThresh, imageConcat);
    cv::imshow("None appoxiation", imageConcat);
    cv::waitKey(0);
    
}