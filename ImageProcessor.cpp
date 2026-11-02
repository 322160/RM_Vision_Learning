#include "ImageProcessor.h"
#include <iostream>
#include <vector>
#include "NMS.h"

cv::Mat ImageProcessor::process(cv::Mat& frame) {
    cv::Mat gray, binary;
    
    // 1. 转灰度图
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // 2. 固定阈值二值化 (低于100变白，高于100变黑)
    // 如果画面偏暗或偏亮，你可以把 100 改成 80 或 120 试试
    cv::threshold(gray, binary, 100, 255, cv::THRESH_BINARY_INV);

    // 3. 形态学闭运算（强行把房子的边缘“糊”连起来，防止断裂）
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
    cv::morphologyEx(binary, binary, cv::MORPH_CLOSE, kernel);

    // 【调试魔法】：把黑白图弹出来，让你看清真相
    cv::imshow("Binary Image", binary);

    // 4. 寻找轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Rect> boxes;
    for (size_t i = 0; i < contours.size(); i++) {
        // 5. 面积过滤（只要大于100像素就抓）
        if (cv::contourArea(contours[i]) > 100) { 
            cv::Rect rect = cv::boundingRect(contours[i]);
            boxes.push_back(rect); 
        }
    }
    
    std::cout << "暴力提取的矩形数量: " << boxes.size() << std::endl; 

    // 6. NMS 去重
    std::vector<cv::Rect> final_boxes = applyNMS(boxes, 0.4f); 
    
    // 7. 画框（洋红色）
    for (const auto& rect : final_boxes) {
        cv::rectangle(frame, rect, cv::Scalar(255, 0, 255), 2); 
    }
    
    return frame;
}