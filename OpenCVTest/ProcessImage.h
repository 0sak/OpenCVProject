#pragma once

#include<iostream>
#include<string>
#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ProcessImage {
public:
    cv::Mat imageGrey,
        imageBlurred, 
        imageThresh, 
        sobel_x, 
        sobel_y, 
        sobelResult, 
        erodeResult, 
        dilateResult,
        imageConcat,
        resizedDown;

    int ddepth = CV_32F;
    int down_height = 800;
    int down_width = 1000;

    cv::Mat processImage(cv::Mat);

};