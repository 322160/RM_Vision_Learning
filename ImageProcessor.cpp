#include "ImageProcessor.h"
#include <iostream>
#include <vector>
#include "NMS.h"

cv::Mat ImageProcessor::process(cv::Mat& frame) {
    cv::Mat hsv, mask;
    
    // 1. 转成 HSV 颜色空间
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // 1. 定义红色的两段 HSV 范围
    // 第一段：偏橙红 (H: 0-10)
    cv::Scalar lower_red1(0, 120, 70);
    cv::Scalar upper_red1(10, 255, 255);
    
    // 第二段：偏紫红 (H: 170-180)
    cv::Scalar lower_red2(170, 120, 70);
    cv::Scalar upper_red2(180, 255, 255);

    // 2. 分别提取两段红色
    cv::Mat mask1, mask2;
    cv::inRange(hsv, lower_red1, upper_red1, mask1);
    cv::inRange(hsv, lower_red2, upper_red2, mask2);

    // 3. 把两段红色的黑白图合并成一张 mask
    cv::bitwise_or(mask1, mask2, mask);

    // 👇 【Day 5 魔法】：形态学降噪（去碎点）
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(20, 20));
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel); 
//CLOSE为椭圆核
    // 4. 寻找轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    std::cout << "形态学降噪后的原始轮廓数量: " << contours.size() << std::endl; 

    // 5. 过滤
    std::vector<cv::Rect> boxes;
    for (size_t i = 0; i < contours.size(); i++) {
        if (cv::contourArea(contours[i]) > 15) { 
            cv::Rect rect = cv::boundingRect(contours[i]);
            float ratio = std::max(rect.width, rect.height) / (float)std::min(rect.width, rect.height);
            if (ratio > 0.3f && ratio < 15.0f) { 
                boxes.push_back(rect);
            }
        }
    }
    std::cout << "过滤后合格的框数量: " << boxes.size() << std::endl; 

    // 6. NMS 去重
    std::vector<cv::Rect> final_boxes = applyNMS(boxes, 0.1f); 
    
    // 7. 画框
    for (const auto& rect : final_boxes) {
        cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2); 
    }
    
    return frame;
}