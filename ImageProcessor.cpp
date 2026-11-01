#include "ImageProcessor.h"
#include <iostream>
#include <vector>
#include "NMS.h"

cv::Mat ImageProcessor::process(cv::Mat& frame) {
    cv::Mat hsv, mask;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

     // 2. 定义白色的 HSV 范围
    // H: 0-180 (白色没有明显色相，所以全范围)
    // S: 0-30 (白色饱和度极低，通常小于30，不能设太高，否则会抓到鲜艳的颜色)
    // V: 200-255 (白色亮度极高)
    cv::Scalar lower_white(0, 10, 50);
    cv::Scalar upper_white(180, 30, 255);

    // 3. 提取白色
    cv::inRange(hsv, lower_white, upper_white, mask);
    // 👇 必须先是 namedWindow，再是 imshow，最后 resizeWindow
    cv::namedWindow("White Mask Debug", cv::WINDOW_NORMAL); // 1. 先定规矩：允许缩放
    cv::resizeWindow("White Mask Debug", 400, 300);         // 2. 再设定大小：400x300
    cv::imshow("White Mask Debug", mask);                   // 3. 最后显示图像


    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
 // 👇 新增：看看找到了多少个原始轮廓
    std::cout << "找到的原始轮廓数量: " << contours.size() << std::endl; 



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
     // 👇 新增：看看过滤后还剩多少
    std::cout << "过滤后合格的框数量: " << boxes.size() << std::endl; 

    std::vector<cv::Rect> final_boxes = applyNMS(boxes, 0.4f); // NMS去重
    
    // 直接把框画在传进来的这一帧图像上
    for (const auto& rect : final_boxes) {
        cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2); 
    }
    
    return frame;
}