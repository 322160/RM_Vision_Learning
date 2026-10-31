#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

// 传入所有框，返回去重后的框
std::vector<cv::Rect> applyNMS(std::vector<cv::Rect>& 
    boxes, float iou_threshold);