#include "ImageProcessor.h"

int main() {
    ImageProcessor processor;
    // 输入是 test.jpg，输出存为 result.jpg
    processor.process("test.jpg", "result.jpg");
    return 0;
}