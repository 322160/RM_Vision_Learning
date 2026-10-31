#pragma once
#include <opencv2/opencv.hpp>
#include <string>

class ImageProcessor {
public:
    // 处理图片的方法，传入输入路径和输出路径
    void process(const std::string& input_path, const std::string& output_path);
};