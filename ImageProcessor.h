#pragma once
#include <opencv2/opencv.hpp>
#include <string>

class ImageProcessor {
public:
      // 接收一帧图像，返回画好框的图像
    cv::Mat process(cv::Mat& frame); 
};