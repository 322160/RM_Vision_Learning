#include "NMS.h"
#include <algorithm>

// 计算两个矩形的交并比 (IoU)
float calculateIoU(const cv::Rect& boxA, const cv::Rect& boxB) {
    int x1 = std::max(boxA.x, boxB.x);
    int y1 = std::max(boxA.y, boxB.y);
    int x2 = std::min(boxA.x + boxA.width, boxB.x + boxB.width);
    int y2 = std::min(boxA.y + boxA.height, boxB.y + boxB.height);
    
    int interArea = std::max(0, x2 - x1) * std::max(0, y2 - y1);
    int unionArea = boxA.area() + boxB.area() - interArea;
    
    if (unionArea <= 0) return 0.0f;
    return (float)interArea / unionArea;
}

std::vector<cv::Rect> applyNMS(std::vector<cv::Rect>& boxes, float iou_threshold) {
    std::vector<cv::Rect> result;
    if (boxes.empty()) return result;

    // 1. 按面积从大到小排序（假设越大的框越有可能是真正的装甲板）
    std::sort(boxes.begin(), boxes.end(), [](const cv::Rect& a, const cv::Rect& b) {
        return a.area() > b.area();
    });

    std::vector<bool> is_deleted(boxes.size(), false);

    // 2. 遍历所有框
    for (size_t i = 0; i < boxes.size(); i++) {
        if (is_deleted[i]) continue;
        
        result.push_back(boxes[i]); // 老大保留

        for (size_t j = i + 1; j < boxes.size(); j++) {
            if (is_deleted[j]) continue;
            // 如果和老大的重叠度极高，干掉小弟
            if (calculateIoU(boxes[i], boxes[j]) > iou_threshold) {
                is_deleted[j] = true;
            }
        }
    }
    return result;
}