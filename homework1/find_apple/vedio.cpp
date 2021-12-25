//
// Created by lhq on 12/21/21.
//
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <assert.h>

int main1() {
    cv::VideoCapture capture("/home/lhq/Downloads/video.mp4");
    cv::Mat src;
    capture >> src;
    assert(!src.empty());
    cv::VideoWriter writer("../1_output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 50,
                           cv::Size(src.cols, src.rows), false);
    // 需要注意的是，这里由于输出灰度图片，参数isColor为false，如果输出彩色图片 则应为true
    while (true) {
        cv::Mat output;
        cv::cvtColor(src, output, cv::COLOR_BGR2GRAY);
        writer << output;
        capture >> src;
        if (src.empty()) break;
    }
    writer.release();
    return 0;
}
