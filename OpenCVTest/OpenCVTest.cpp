#include<iostream>
#include<opencv2/opencv.hpp>
#include<string>
#include <opencv2/imgproc/imgproc.hpp>

int down_height = 800;
int down_width = 1000;

int main(int argc, char** argv)
{
    for (int i = 1; i < 6; i++) {
        cv::Mat imageGrey;
        cv::Mat image;
        cv::Mat imageBlurred;
        cv::Mat imageThresh;
        cv::Mat imageConcat;
        cv::Mat sobel_x, sobel_y;
        cv::Mat sobelResult;
        cv::Mat erodeResult; 
        cv::Mat dilateResult;
        cv::Mat resizedDown;

        int ddepth = CV_32F;

        std::string imageToRead = "images/";

        imageToRead += std::to_string(i);
        imageToRead += ".png";

        std::cout << imageToRead << std::endl;

        image = cv::imread(imageToRead);

        if (image.empty()) {
            std::cout << "Image not Found !!!" << std::endl;
            std::cin.get();
        }

        cv::resize(image, resizedDown, cv::Size(down_width, down_height), cv::INTER_LINEAR);
        cv::cvtColor(resizedDown, imageGrey, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(imageGrey, imageBlurred, cv::Size(3, 3), 0);
        cv::threshold(imageBlurred, imageThresh, 127, 255, cv::THRESH_BINARY);
        cv::Sobel(imageThresh, sobel_x, ddepth, 1, 0, 3, cv::BORDER_DEFAULT);
        cv::Sobel(imageThresh, sobel_y, ddepth, 0, 1, 3, cv::BORDER_DEFAULT);
        cv::magnitude(sobel_x, sobel_y, sobelResult);
        
        cv::erode(sobelResult, erodeResult, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));
        cv::dilate(erodeResult, dilateResult, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));
        
        cv::hconcat(dilateResult, sobelResult, imageConcat);

        cv::imshow(imageToRead, imageConcat);

        cv::waitKey(0);
    }

    return 0;
}