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
    cv::drawContours(image_copy, contours, -1, cv::Scalar(0, 255, 0), 2);
    cv::imshow("None appoxiation", image_copy);
    cv::waitKey(0);
}