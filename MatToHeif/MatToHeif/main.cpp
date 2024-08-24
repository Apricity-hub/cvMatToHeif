#include <iostream>
#include "MatToHeif.h"
#include "HeifToMat.h"

int main()
{
    // RGB
    cv::Mat matRGB;
    cv::Mat matBGR = cv::imread("C:\\Users\\Lenovo\\Desktop\\opencv.png");
    const char* rgbFilename = "C:\\Users\\Lenovo\\Desktop\\opencv.heif";
    const char* dstRgbFilename = "C:\\Users\\Lenovo\\Desktop\\opencv_decode.png";

    cv::cvtColor(matBGR, matRGB, cv::COLOR_BGR2RGB);
    bool isRGB = RGBMatToHeif(matRGB, rgbFilename);

    matBGR = HeifToRGBMat(rgbFilename);
    cv::imwrite(dstRgbFilename, matBGR);


    // GRAY
    cv::Mat matGRAY = cv::imread("C:\\Users\\Lenovo\\Desktop\\opencv_gray.png", cv::IMREAD_GRAYSCALE);
    const char* grayFilename = "C:\\Users\\Lenovo\\Desktop\\opencv_gray.heif";
    const char* dstGrayFilename = "C:\\Users\\Lenovo\\Desktop\\opencv_gray_decode.png";

    bool isGRAY = GRAYMatToHeif(matGRAY, grayFilename);


    cv::Mat dstMatGRAY = HeifToGRAYMat(grayFilename);

    cv::imwrite(dstGrayFilename, dstMatGRAY);


    return 0;
}