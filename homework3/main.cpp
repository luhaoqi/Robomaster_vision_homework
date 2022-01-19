#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace cv;

namespace work1 {
    Mat img, src, result;

    void on_mouse(int event, int x, int y, int flags, void *ustc) {
        char temp[16];
        img = src.clone();
        if (event == EVENT_LBUTTONDOWN) {
            std::vector<Point2f> *src_pt = (std::vector<Point2f> *) ustc;
            sprintf(temp, "(%d,%d)", x, y);
            src_pt->push_back(Point2f(x, y));
            for (auto u:*src_pt)
                circle(img, u, 5, Scalar(0, 0, 255), -1);
            putText(img, temp, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));

            imshow("input", img);
        }
    }

    void doPerspectiveTransform(Mat input, Mat &output) {
        std::vector<Point2f> src_pt, dst_pt(4);
        output = input.clone();
        namedWindow("input");
        imshow("input", input);
        setMouseCallback("input", on_mouse, &src_pt);

        waitKey(0);

        for (int i = 0; i < 4; ++i)
            std::cout << src_pt[i].x << ' ' << src_pt[i].y << std::endl;

        dst_pt = {{0,   0},
                  {0,   200},
                  {600, 200},
                  {600, 0}};

        Mat warp_matrix = getPerspectiveTransform(src_pt, dst_pt);

        warpPerspective(input, output, warp_matrix, Size(600, 200));
    }

    void solve() {
        src = imread("../data pack/car.jpg");
        result;
        std::cout << "从左上角开始逆时针标点，按任意键结束" << std::endl;
        doPerspectiveTransform(src, result);

        namedWindow("output");
        imshow("output", result);

        waitKey(0);
    }
}
namespace work2 {
    const int board_x = 9, board_y = 6;
    const int board_n = board_x * board_y;
    Size board_size(board_x, board_y);
    Size cell_size(10, 10);
    Mat image_ori, image_gray, image_draw;
    int found;

    std::vector<Point2f> points;
    std::vector<std::vector<Point2f> > points_class;

    std::vector<Point3f> world_points;
    std::vector<std::vector<Point3f> > world_points_class;

    Mat camera_matrix = (Mat_<double>(3, 3) << 9.1234465679824348e+02, 0., 6.4157634413436961e+02,
            0., 7.6573154962089018e+02, 3.6477945186797331e+02,
            0., 0., 1.);
    Mat dist_coeffs = (Mat_<double>(1, 5)
            << 0., -4.2669718747763807e-01, 2.6509688616309912e-01, -5.3624979910268683e-04, -4.1011485564833132e-04);
    Mat rvecs;
    Mat tvecs;

    void solve() {
        Size image_size;
        int cnt = 0;

        for (int j = 0; j < board_y; j++) {
            for (int k = 0; k < board_x; k++) {
                Point3f pt;
                pt.x = k * cell_size.width;
                pt.y = j * cell_size.height;
                pt.z = 0;
                world_points.push_back(pt);
            }
        }
        int success = 0;
        for (int i = 0; i < 41; i++) {
            image_ori = cv::imread("../data pack/chess/" + std::__cxx11::to_string(i) + ".jpg");
            if (!cnt) {
                image_size.width = image_ori.cols;
                image_size.height = image_ori.rows;
            }
            cnt++;
            found = findChessboardCorners(image_ori, board_size, points);
            if (found && points.size() == board_n) {
                success++;
                cvtColor(image_ori, image_gray, COLOR_BGR2GRAY);
                find4QuadCornerSubpix(image_gray, points, Size(5, 5));
                solvePnP(world_points, points, camera_matrix, dist_coeffs, rvecs, tvecs);
                std::cout << "image:" << i << std::endl;
                std::cout << "rvec: " << rvecs << std::endl;
                std::cout << "tvec: " << tvecs << std::endl;
                points_class.push_back(points);
                image_draw = image_ori.clone();
                drawChessboardCorners(image_draw, board_size, points, found);
                imshow("corners", image_draw);
                int key = waitKey(0);
                if (char(key) == 'q') {
                    break;
                }
            } else {
                std::cout << "failed with picture " + i << std::endl;
            }
            points.clear();
        }
    }
}

int main() {
    int type;
    std::cin >> type;
    if (type == 1) work1::solve();
    else if (type == 2) work2::solve();
    return 0;
}