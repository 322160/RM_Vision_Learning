#include <iostream>
#include <opencv2/opencv.hpp>
#include "ImageProcessor.h"

int main() {
    // 1. 打开视频文件
    cv::VideoCapture cap("test.mp4");
    
    if (!cap.isOpened()) { 
        std::cout << "错误：打不开视频，检查一下是不是没放进文件夹！" << std::endl; 
        return -1; 
    }

    ImageProcessor processor;
    cv::Mat frame;

    // 允许窗口缩放（解决窗口太大拖不动的问题）
    cv::namedWindow("Robot Vision Stream", cv::WINDOW_NORMAL);

    // 2. 死循环：不断地读、处理、显示
    while (true) {
        double t_read_start = (double)cv::getTickCount();
        cap >> frame; // 读取下一帧
        double t_read_end = (double)cv::getTickCount();
        
        if (frame.empty()) {
            std::cout << "视频播放完毕！" << std::endl;
            break; 
 // 核心魔法：把视频的读取指针重置回第 0 帧
            cap.set(cv::CAP_PROP_POS_FRAMES, 0); 
            continue; // 跳过本次循环剩余代码，重新读第一帧
        }

        // 【提速魔法】：把画面缩小一半（0.5）。如果还卡，就改成 0.25
        cv::resize(frame, frame, cv::Size(), 0.5, 0.5);

        // 计时处理时间
        double t_process_start = (double)cv::getTickCount();
        processor.process(frame); 
        double t_process_end = (double)cv::getTickCount();

        // 计算耗时（毫秒）
        double read_ms = (t_read_end - t_read_start) / cv::getTickFrequency() * 1000;
        double process_ms = (t_process_end - t_process_start) / cv::getTickFrequency() * 1000;

        // 计算总 FPS
        double fps = 1.0 / ((read_ms + process_ms) / 1000.0);

        // 把数据打在画面上
        cv::putText(frame, "FPS: " + std::to_string((int)fps), 
                    cv::Point(20, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
        cv::putText(frame, "Read: " + std::to_string((int)read_ms) + "ms  Process: " + std::to_string((int)process_ms) + "ms", 
                    cv::Point(20, 60), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);

        // 显示画面
        cv::imshow("Robot Vision Stream", frame);

        // 把窗口固定在 800x600 的大小
        cv::resizeWindow("Robot Vision Stream", 800, 600);

        // 按 'q' 退出
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
    
    cap.release(); 
    cv::destroyAllWindows();
    return 0;
}