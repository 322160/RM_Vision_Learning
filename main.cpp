#include <iostream>
#include <opencv2/opencv.hpp>
#include "ImageProcessor.h"

int main() {
    // 1. 打开视频文件 (如果要开真实摄像头，把 "test.mp4" 改成数字 0)
    cv::VideoCapture cap("test.mp4");
    
    if (!cap.isOpened()) { 
        std::cout << "错误：打不开视频，检查一下是不是没放进文件夹！" << std::endl; 
        return -1; 
    }

    ImageProcessor processor;
    cv::Mat frame;

    // 2. 死循环：不断地读、处理、显示
    while (true) {
        cap >> frame; // 读取视频的下一帧
        
        if (frame.empty()) {
            std::cout << "视频播放完毕！" << std::endl;
            break; // 读完了就退出
        }

   // 👇 核心新加代码：开始计时！
        double t1 = (double)cv::getTickCount();




        // 3. 交给我们的类处理
        processor.process(frame); 

 // 👇 核心新加代码：结束计时，并算出 FPS！
        double t2 = (double)cv::getTickCount();
        double fps = cv::getTickFrequency() / (t2 - t1);


          // 把 FPS 用绿色的字，打在你的视频画面左上角
        cv::putText(frame, "FPS: " + std::to_string((int)fps), 
                    cv::Point(20, 50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);

        // 4. 弹窗显示画面
        cv::imshow("Robot Vision Stream", frame);

        // 5. 关键点：waitKey(1) 控制播放速度，不能是0！
        // 如果按下键盘的 'q' 键，就提前退出
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
    
    cap.release(); // 释放视频
    cv::destroyAllWindows();
    return 0;
}