#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;
using namespace cv::dnn;

int main() {
    // 1. 加载 YOLOv8 模型
    Net net = readNetFromONNX("yolov8n.onnx");
    if (net.empty()) {
        cout << "模型加载失败，请检查文件！" << endl;
        return -1;
    }
    cout << "YOLOv8 模型加载成功！" << endl;

    // 2. 读取测试图片
    Mat frame = imread("dog.jpg");
    if (frame.empty()) return -1;

    // 3. 预处理（YOLOv8 需要 640x640 输入）
    Mat blob;
    blobFromImage(frame, blob, 1.0 / 255.0, Size(640, 640), Scalar(0, 0, 0), true, false);
    net.setInput(blob);
    
    // 4. 推理
    Mat output = net.forward();
    output = output.reshape(1, 84); 

    vector<Rect> boxes;
    vector<float> confidences;

    // 5. 解析输出
    for (int i = 0; i < 8400; i++) {
        float cx = output.at<float>(0, i);
        float cy = output.at<float>(1, i);
        float w = output.at<float>(2, i);
        float h = output.at<float>(3, i);

        float maxConf = 0;
        for (int c = 4; c < 84; c++) {
            float conf = output.at<float>(c, i);
            if (conf > maxConf) maxConf = conf;
        }

        if (maxConf > 0.3) {
            int left = int((cx - w / 2.0) * frame.cols / 640.0);
            int top = int((cy - h / 2.0) * frame.rows / 640.0);
            int width = int(w * frame.cols / 640.0);
            int height = int(h * frame.rows / 640.0);

            boxes.push_back(Rect(left, top, width, height));
            confidences.push_back(maxConf);
        }
    }

    // 6. NMS 去重
    vector<int> indices;
    NMSBoxes(boxes, confidences, 0.3, 0.45, indices);

    cout << "最终保留了 " << indices.size() << " 个框！" << endl;

    // 7. 画框
    for (int idx : indices) {
        rectangle(frame, boxes[idx], Scalar(0, 0, 255), 2);
        string label = "Conf: " + to_string(confidences[idx]).substr(0, 4);
        putText(frame, label, Point(boxes[idx].x, boxes[idx].y - 5), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1);
    }

    // 8. 缩小窗口显示
    Mat displayFrame;
    resize(frame, displayFrame, Size(800, frame.rows * 800 / frame.cols));
    imshow("YOLOv8 Modern AI", displayFrame);
    waitKey(0);

    return 0;
}