#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <assert.h>

int red_h1 = 10, red_h2 = 156, red_s = 43, red_v = 46, open_size = 3, close_size = 3;
const char *win_name = "set_parameter";
cv::Mat src;

void on_trackbar(int, void *) {
    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV); // 将颜色空间从BGR转为HSV
    cv::Mat hsv_part1, hsv_part2;
    cv::inRange(hsv, cv::Scalar(0, red_s, red_v), cv::Scalar(red_h1, 255, 255), hsv_part1);
    cv::inRange(hsv, cv::Scalar(red_h2, red_s, red_v), cv::Scalar(180, 255, 255),
                hsv_part2); // 提取红色
    cv::Mat ones_mat = cv::Mat::ones(cv::Size(src.cols, src.rows), CV_8UC1);
    cv::Mat hsv_result = 255 * (ones_mat - (ones_mat - hsv_part1 / 255).mul(
            ones_mat - hsv_part2 / 255)); // 对hsv_part1的结果和hsv_part2的结果取并集

    // open operation
    if (open_size > 0) {
        cv::Mat element1 = getStructuringElement(cv::MORPH_RECT, cv::Size(open_size, open_size));
        cv::morphologyEx(hsv_result, hsv_result, cv::MORPH_OPEN, element1);
    }

    // close operation
    if (close_size > 0) {
        cv::Mat element2 = getStructuringElement(cv::MORPH_RECT, cv::Size(close_size, close_size));
        cv::morphologyEx(hsv_result, hsv_result, cv::MORPH_CLOSE, element2);
    }

    // find & filtrate & draw contours
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(hsv_result, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    std::vector<cv::Rect> boundRect(contours.size());
    cv::Mat src_result = src.clone(); // warning: should not draw on the src, because src is global valuable
    for (int i = 0; i < contours.size(); i++) {
        boundRect[i] = boundingRect(cv::Mat(contours[i]));
        // only draw the rectangle which is big enough
        if (boundRect[i].width > 50 && boundRect[i].height > 50) {
            rectangle(src_result, cv::Point(boundRect[i].x, boundRect[i].y),
                      cv::Point(boundRect[i].x + boundRect[i].width, boundRect[i].y + boundRect[i].height),
                      cv::Scalar(255, 0, 0), 2, 8);
        }
    }

    cv::imshow(win_name, hsv_result);
    cv::imshow("apple", src_result);
}

int main(int argc, char **argv) {
    src = cv::imread("../apple.png");
    assert(src.channels() == 3); // 检测是否为三通道彩色图片
    namedWindow(win_name, cv::WINDOW_AUTOSIZE);
    // trackbars for setting the parameters
    cv::createTrackbar("red_h1:", win_name, &red_h1, 50, on_trackbar);
    cv::createTrackbar("red_h2:", win_name, &red_h2, 180, on_trackbar);
    cv::createTrackbar("red_s:", win_name, &red_s, 200, on_trackbar);
    cv::createTrackbar("red_v:", win_name, &red_v, 100, on_trackbar);
    cv::createTrackbar("open_size:", win_name, &open_size, 20, on_trackbar);
    cv::createTrackbar("close_size:", win_name, &close_size, 20, on_trackbar);
    cv::waitKey(0);
    return 0;
}