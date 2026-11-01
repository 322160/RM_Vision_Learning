 docs/zekai_zhu
# 我的视觉学习记录
今天完成了视频流的处理，并且学会了使用 Git 分支！

# RoboMaster 视觉入门工程 (C++ / OpenCV)

本项目是备战 RoboMaster 视觉组的大一学习工程，旨在从零搭建一个标准化的 C++ 计算机视觉流水线。

## 📅 开发日志 (Changelog)
*   **Day 3 (当前)**：完成基于 HSV 色彩空间的颜色提取、长宽比物理过滤，并用 NMS（非极大值抑制）算法实现重叠框去重。
*   **Day 4 (计划)**：接入视频流实时处理（VideoCapture），解决多线程防卡顿问题。
*   *(后续更新...)*

## 🛠️ 核心功能
1.  **图像预处理**：BGR 转 HSV 色彩空间，进行颜色阈值过滤（`cv::inRange`）。
2.  **目标提取**：寻找色块轮廓（`cv::findContours`）并计算外接矩形。
3.  **逻辑过滤**：根据目标面积和长宽比（Aspect Ratio）剔除环境噪点。
4.  **后处理去重**：手写 NMS 算法（基于 IoU 交并比），剔除重叠的候选框。

## 📂 工程目录结构
```text
├── main.cpp              # 主程序入口
├── ImageProcessor.h      # 图像处理类声明
├── ImageProcessor.cpp    # 图像处理核心逻辑实现
├── NMS.h                 # NMS 算法声明
├── NMS.cpp               # NMS 算法实现
├── CMakeLists.txt        # CMake 构建配置
├── test.jpg              # 测试输入图像
└── result.jpg            # 识别结果输出图像
 main
