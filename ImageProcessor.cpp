#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <vector>

using namespace std;
using namespace cv;
using namespace cv::dnn;

int main() {
    // 1. 加载 Caffe 模型
    string prototxt = "deploy.prototxt";
    string caffemodel = "mobilenet_iter_73000.caffemodel";
    Net net = readNetFromCaffe(prototxt, caffemodel);
    if (net.empty()) {
        cout << "模型加载失败，请检查文件路径！" << endl;
        return -1;
    }
    cout << "模型加载成功！按 'q' 键退出..." << endl;

    // 2. 打开视频文件（请确保你的视频命名为 test.mp4 并放在同一目录下）
    VideoCapture cap("test.mp4");
    if (!cap.isOpened()) {
        cout << "视频读取失败，请检查视频文件名！" << endl;
        return -1;
    }

    Mat frame;
    // 3. 循环读取视频的每一帧
    while (true) {
        cap >> frame;
        if (frame.empty()) break; // 视频播放完毕

        // 4. 图像预处理，转成 Blob 格式 (SSD 模型标准输入是 300x300)
        Mat blob;
        blobFromImage(frame, blob, 1.0, Size(300, 300), Scalar(104, 117, 123), false, false);

        // 5. 丢进神经网络推理
        net.setInput(blob);
        Mat detection = net.forward();

        // 6. 解析输出并画框
        Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
        for (int i = 0; i < detectionMat.rows; i++) {
            float confidence = detectionMat.at<float>(i, 2); // 置信度
            if (confidence > 0.5) { // 只要置信度大于 0.5 的
                int idx = detectionMat.at<float>(i, 1);
                int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
                int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
                int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
                int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);
                
                // 画绿框
                Rect object((int)xLeftBottom, (int)yLeftBottom, (int)(xRightTop - xLeftBottom), (int)(yRightTop - yLeftBottom));
                rectangle(frame, object, Scalar(0, 255, 0), 2);
            }
        }

        // 7. 显示画面
        imshow("AI Video Stream", frame);
        
        // 视频播放控制，按 q 退出
        if (waitKey(1) == 'q') break;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}