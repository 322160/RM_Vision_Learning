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
       while (cap.read(frame)) {
        Mat blob;
        // 注意：Caffe 模型通常用 BGR，swapRB 写 false
        blobFromImage(frame, blob, 1.0, Size(300, 300), Scalar(104, 117, 123), false, false);
        net.setInput(blob);
        Mat detection = net.forward();
        Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

        for (int i = 0; i < detectionMat.rows; i++) {
            float confidence = detectionMat.at<float>(i, 2); 
            int classId = static_cast<int>(detectionMat.at<float>(i, 1));

            if (confidence > 0.2 && classId != 0) { 
                int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
                int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
                int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
                int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);
                
                // 算一下这个框的面积
                int w = x2 - x1;
                int h = y2 - y1;
                
                // 【核心过滤】：如果框太大（占画面 30% 以上），直接扔掉，那是误检！
                if (w * h < frame.cols * frame.rows * 0.3) {
                    rectangle(frame, Rect(x1, y1, w, h), Scalar(0, 0, 255), 2);
                }
            }
        }

        // 【核心魔法】：在显示前，直接把原图缩小一半，窗口绝对变小！
        Mat displayFrame;
        cv::resize(frame, displayFrame, cv::Size(), 0.5, 0.5);

        imshow("AI Video Stream", displayFrame);
        if (waitKey(1) == 'q') break;
    }
    
    cap.release(); 
    cv::destroyAllWindows();
 cv::namedWindow("AI Image Output", cv::WINDOW_NORMAL);
    cv::resizeWindow("AI Image Output", 800, 600);
    imshow("AI Image Output", frame);
    
    waitKey(0);



    return 0;
}