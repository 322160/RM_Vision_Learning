#include "ImageProcessor.h"
#include <iostream>
#include <vector>
#include "NMS.h"

void ImageProcessor::process(const std::string& input_path, const std::string& output_path) {
    cv::Mat img = cv::imread(input_path);
    if (img.empty()) {
        std::cout << "错误：找不到图片 " << input_path << std::endl;
        return;
    }

    cv::Mat hsv, mask;
    // 1. 转成 HSV 颜色空间
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);

       // 1. 转成 HSV 颜色空间
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);

    // 2. 定义蓝色的 HSV 范围
    // H在100-130左右，为了过滤灰暗的墙，把S(饱和度)和V(亮度)的门槛拉高
    cv::Scalar lower_blue(90, 50, 50); 
    cv::Scalar upper_blue(130, 255, 255);
    
    // 3. 提取蓝色
    cv::inRange(hsv, lower_blue, upper_blue, mask);
    // 3. 寻找轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 4. 过滤
    std::vector<cv::Rect> candidate_boxes;
    for (size_t i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        if (area > 500) { 
            cv::Rect rect = cv::boundingRect(contours[i]);
            
            float max_val = std::max(rect.width, rect.height);
            float min_val = std::min(rect.width, rect.height);
            float ratio = max_val / min_val;

            // 放宽一点长宽比要求
            if (ratio > 0.1f && ratio < 10.0f) {
                candidate_boxes.push_back(rect); 
            }
        }
    }

    // 5. NMS 去重
    std::cout << "长宽比过滤后的候选框数量: " << candidate_boxes.size() << std::endl;
    std::vector<cv::Rect> final_boxes = applyNMS(candidate_boxes, 0.01f);
    std::cout << "NMS 过滤后的最终框数量: " << final_boxes.size() << std::endl;

    // 6. 画框
    for (const auto& rect : final_boxes) {
        cv::rectangle(img, rect, cv::Scalar(0, 255, 0), 2);
    }

    // 7. 保存并显示
    cv::imwrite(output_path, img);
    std::cout << "处理完成！已保存为: " << output_path << std::endl;

    cv::imshow("Original with Box", img);
    cv::imshow("Blue Mask", mask);
    cv::waitKey(0);
}