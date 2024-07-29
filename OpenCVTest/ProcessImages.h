#pragma once

#include<string>
#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/utils/logger.hpp>
#include "wtypes.h"
#include "Metric.h"
#include <cmath>

const int DARKNESS_THRESHOLD = 100;
const float WHITE_PIXEL_TRESHOLD_LOW = 0.4700;
const float WHITE_PIXEL_THRESHOLD_HIGH = 0.800;
const float ASPECT_RATIO_MIN = 1.9f;
const float ASPECT_RATIO_MAX = 15.0f;
const float FALSE_POSITIVE_THRESHOLD = 100.0f;

class ProcessImages {
public:
    std::string imageToRead;
    int ddepth = CV_8U;

    int desktopWidth;
    int desktopHeigth;
    Metric metric;
    int index = 0;
    float minAreaOuter, maxAreaOuter, minAreaInner, maxAreaInner;

    bool DEBUG_FLAG_SHOW_RESULT = false;

    cv::Mat image,
        imageGrey,
        imageBlurred,
        imageThresh,
        imageThreshResized,
        sobelX,
        sobelY,
        sobelResult,
        erodeResult,
        dilateResult,
        imageConcat,
        resizedDown,
        res,
        newImage;

    std::vector<std::vector<cv::Point>> contours, contoursSecond;
    std::vector<cv::Vec4i> hierarchy, hierarchySecond;

    /// @brief reads in dataset_labeling.txt and sets total amount of license plates and characters that can be recognized.
    void initLabels();

    /// @brief Function to apply multiple filters to the original picture, so that it is ready for extracting all of its contours. 
    /// The results are saved in the matrixes of this class. (Note that there are some that are not being used, we tried out several methods to preprocess our pictures)
    /// @param image 
    void preprocessImage(cv::Mat image);

    /// @brief Function to start the license plate recognition, it loops over all existing pictures from the dataset and calls necessary functions of this class. 
    /// Also adds height and width to labels.
    void execute();

    /// @brief prints filter conditions for the current picture that is analyzed e.g Min Area Outer Contour (Rectangle).
    /// @param i 
    void printFilterConditions(int i);

    /// @brief Set width, height of a picture and calculate relative min and max Area for outer contours in relation with image width and height.
    void filterImageData();

    /// @brief First we start by extracting all of the contours of a picture, secondly the perimeter of a contour is calculated and the contour is approxymated 
    /// (for instance a rectangle can be drown with 4 vertices). After that we check if the contour in fact contains only 4 points, indicating that it is a rectangle. 
    /// We filter the extracted rectangle by area and apect ratio. The next step is to see how many white pixels exist in the contour, 
    /// since a license plate will consist of a lot of white pixels and some black pixels indicating the letters. 
    /// If the amount of white pixels is high enough we again filter for all countours that exist in our rectangle. 
    /// In this step we cant to see if we cann figure out the letters on our license plate. If the contour has a specific area we let it pass and count it.
    ///  Since a license plate will have about 15 letters on it we check if we found 4-12 contours that fit our area check. If so we sucessfully found a licenseplate on the picture.
    /// @param brighter 
    void filterContours(bool brighter);

    /// @brief Calculate relative min and max area for inner Contour
    /// @param subImgCopy 
    void setFilterConditionsInnerContours(cv::Mat& subImgCopy);

    /// @brief Display the picture with recognized license plates and the corresponding threshold image.
    /// @param image_copy 
    void displayResults(cv::Mat& image_copy);

    /// @brief When outer the outer contour survives all of our checks, count all contours inside the rectangle with a specific area
    /// @param contourCounter 
    /// @param i 
    /// @param subImgCopy 
    void countInnerContours(unsigned int& contourCounter, int i, cv::Mat& subImgCopy);

    /// @brief print Area, Number of white pixels and ratio of white and black pixels in rectangle
    /// @param i 
    /// @param resultRect 
    /// @param whitePixelsInRectangle 
    /// @param proportionAreaWhitepixels 
    void printCandidateData(int i, cv::Rect2f& resultRect, double whitePixelsInRectangle, float proportionAreaWhitepixels);

    /// @brief Check aspect ratio and area of cadidate. Ignore if not in these bounderies.
    /// @param resultRect 
    /// @param retflag 
    bool isRectangle(cv::Rect2f& resultRect);

    /// @brief Change from BGR to HSV and check value across all pixels. Decide from value if image is bright or not. Also save brightness in labels.
    /// @param image 
    /// @return 
    bool isImageBright(cv::Mat image);

    /// @brief Make image brighter with alpha and beta values
    /// @param alpha 
    /// @param beta 
    void makeBrighter(double alpha, int beta);

    /// @brief Make image brighter with gamma value
    /// @param img 
    /// @param gamma_ 
    void gammaCorrection(const cv::Mat& img, const double gamma_);

    /// @brief store desktop resolution
    void getDesktopResolution();

    /// @brief Reads in image, makes it brighter if neccacary and starts filtering contours.
    void process();

    /// @brief Calculates distance between to points om a picture
    /// @param x1 
    /// @param y1 
    /// @param x2 
    /// @param y2 
    /// @return 
    float distanceBetweenTwoPoints(float x1, float y1, float x2 , float y2);
};