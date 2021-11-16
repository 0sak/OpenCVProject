#include "ProcessImage.h"

const int NUMBER_OF_IMAGES = 5;

int main(int argc, char** argv)
{
    cv::Mat image;
    ProcessImage pc;

    for (int i = 1; i < NUMBER_OF_IMAGES + 1; i++) {

        std::string imageToRead = "images/";
        imageToRead += std::to_string(i);
        imageToRead += ".png";

        image = cv::imread(imageToRead);

        if (image.empty()) {
            std::cout << "Image not Found !!!" << std::endl;
            std::cin.get();
        }

        image = pc.processImage(image);

        cv::imshow(imageToRead, pc.sobelResult);

        cv::waitKey(0);
    }

    return 0;
}