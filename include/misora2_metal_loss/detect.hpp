#ifndef DETECT_H
#define DETECT_H

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <regex>
#include <limits>

class Detect {
public:
    static cv::Mat preprocessAndCrop(const cv::Mat& image, int x, int y, int width, int height);
    static double extractNumberFromImage(const cv::Mat& thresh);
};


#endif