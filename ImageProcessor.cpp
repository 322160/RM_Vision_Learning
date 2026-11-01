#include "ImageProcessor.h"
#include <iostream>
#include <vector>
#include "NMS.h"

cv::Mat ImageProcessor::process(cv::Mat& frame) {
    cv::Mat hsv, mask;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // 找红色 (如果视频是找蓝色，自己改一下范围)
    cv::Scalar lower_red(0, 100, 100);
    cv::Scalar upper_red(10, 255, 255);
    cv::inRange(hsv, lower_red, upper_red, mask);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Rect> boxes;
    for (size_t i = 0; i < contours.size(); i++) {
        if (cv::contourArea(contours[i]) > 100) { // 面积过滤
            cv::Rect rect = cv::boundingRect(contours[i]);
            float ratio = std::max(rect.width, rect.height) / (float)std::min(rect.width, rect.height);
            if (ratio > 0.5f && ratio < 4.0f) { // 长宽比过滤
                boxes.push_back(rect);
            }
        }
    }

    std::vector<cv::Rect> final_boxes = applyNMS(boxes, 0.4f); // NMS去重
    
    // 直接把框画在传进来的这一帧图像上
    for (const auto& rect : final_boxes) {
        cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2); 
    }
    
    return frame;
}