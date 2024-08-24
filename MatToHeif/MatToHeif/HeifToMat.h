#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat HeifToRGBMat(const char* filename);

cv::Mat HeifToGRAYMat(const char* filename);
