#include "ImageProcessor.h"
#include <iostream>
#include <vector>
#include "NMS.h"

cv::Mat ImageProcessor::process(cv::Mat& frame) {
    cv::Mat hsv, mask;
    
    // 1. 转成 HSV 颜色空间
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // 2. 定义白色的 HSV 范围
    // H: 0-180, S: 0-30 (低饱和), V: 100-255 (高亮度)
    cv::Scalar lower_white(0, 10, 50);
    cv::Scalar upper_white(180, 30, 255);

    // 3. 提取白色
    cv::inRange(hsv, lower_white, upper_white, mask);

    // 👇 【Day 5 魔法】：形态学去噪（为了干掉那 1600 个碎点）
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel); // 开运算去噪点

    // 4. 寻找轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    std::cout << "形态学降噪后的原始轮廓数量: " << contours.size() << std::endl; 

    // 5. 过滤
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
    std::cout << "过滤后合格的框数量: " << boxes.size() << std::endl; 

    // 6. NMS 去重
    std::vector<cv::Rect> final_boxes = applyNMS(boxes, 0.4f); 
    
    // 7. 画框
    for (const auto& rect : final_boxes) {
        cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2); 
    }
    
    return frame;
}