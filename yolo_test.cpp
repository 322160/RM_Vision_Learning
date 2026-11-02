#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <vector>

using namespace std;
using namespace cv;
using namespace cv::dnn;

int main() {
    // 1. 加载模型
    Net net = readNetFromCaffe("deploy.prototxt", "mobilenet_iter_73000.caffemodel");
    if (net.empty()) {
        cout << "模型加载失败，请检查文件！" << endl;
        return -1;
    }
    cout << "模型加载成功！" << endl;

    // 2. 读取狗狗图片（注意：确保你的文件夹里有一张叫 dog.jpg 的图片）
    Mat frame = imread("dog.jpg");
    if (frame.empty()) {
        cout << "图片读取失败，请检查 dog.jpg 是否存在！" << endl;
        return -1;
    }

    // 3. 图像预处理
    Mat blob;
    blobFromImage(frame, blob, 1.0, Size(300, 300), Scalar(104, 117, 123), true, false);
    
    // 4. 推理
    net.setInput(blob);
    Mat detection = net.forward();
    
    // 5. 解析结果
    Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
    
    for (int i = 0; i < detectionMat.rows; i++) {
        float confidence = detectionMat.at<float>(i, 2); 
        int classId = static_cast<int>(detectionMat.at<float>(i, 1));
        
        // 置信度门槛 0.3，且不能是背景（0）
        if (confidence > 0.1&& classId != 0) { 
            int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
            int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
            int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
            int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);
            
            // 修复坐标，防止出界
            x1 = max(0, min(x1, frame.cols - 1));
            y1 = max(0, min(y1, frame.rows - 1));
            x2 = max(0, min(x2, frame.cols - 1));
            y2 = max(0, min(y2, frame.rows - 1));
            
            int w = x2 - x1;
            int h = y2 - y1;
            
            // 过滤离谱大框和极小噪点
            if (w > 10 0&& h > 100 && w < frame.cols * 0.99 && h < frame.rows * 0.99) {
                rectangle(frame, Rect(x1, y1, w, h), Scalar(0, 0, 255), 2);
                
                string label = "ID:" + to_string(classId) + " " + to_string(confidence).substr(0, 4);
                putText(frame, label, Point(x1, y1 - 5), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1);
                
                cout << "找到了！类别: " << classId << " | 置信度: " << confidence << endl;
            }
        }
    }
    
    // 6. 强行把窗口缩小，防止占满屏幕
    Mat displayFrame;
    resize(frame, displayFrame, Size(800, frame.rows * 800 / frame.cols));
    
    imshow("AI Dog Detection", displayFrame);
    
    // 图片显示，按任意键关闭
    waitKey(0);
    return 0;
}