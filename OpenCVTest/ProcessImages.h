#pragma once

#include<iostream>
#include<string>
#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

const int NUMBER_OF_IMAGES = 3;

class ProcessImages {
public:
    std::string imageToRead;
    int ddepth = CV_8U;
    int down_height = 800;
    int down_width = 1000;

    cv::Mat image, 
        imageGrey,
        imageBlurred, 
        imageThresh, 
        sobel_x, 
        sobel_y, 
        sobelResult, 
        erodeResult, 
        dilateResult,
        imageConcat,
        resizedDown;

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    void processImage(cv::Mat);
    void processAll();
    void getContours();

};