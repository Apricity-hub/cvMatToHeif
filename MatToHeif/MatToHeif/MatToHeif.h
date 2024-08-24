#pragma once
#include <opencv2/opencv.hpp>

bool RGBMatToHeif(cv::Mat& mat, const char* filename);

bool GRAYMatToHeif(cv::Mat& mat, const char* filename);