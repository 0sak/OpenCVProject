#include "ProcessImage.h"

cv::Mat ProcessImage::processImage(cv::Mat image) {

    cv::erode(image, erodeResult, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
    cv::dilate(erodeResult, dilateResult, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

    //cv::resize(dilateResult, resizedDown, cv::Size(down_width, down_height), cv::INTER_LINEAR);

    cv::cvtColor(dilateResult, imageGrey, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(imageGrey, imageBlurred, cv::Size(3, 3), 0);

    cv::threshold(imageBlurred, imageThresh, 127, 255, cv::THRESH_BINARY);

    cv::Sobel(imageThresh, sobel_x, ddepth, 1, 0, 3, cv::BORDER_DEFAULT);
    cv::Sobel(imageThresh, sobel_y, ddepth, 0, 1, 3, cv::BORDER_DEFAULT);
    cv::magnitude(sobel_x, sobel_y, sobelResult);

    //cv::hconcat(imageGrey, sobelResult, imageConcat);

    return image;
}