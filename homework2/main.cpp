#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

int main() {

    const int board_w = 9, board_h = 6;
    const int board_n = board_w * board_h;
    Size board_size( 9, 6 );

    Mat gray_img, drawn_img;
    std::vector< Point2f > point_pix_pos_buf;
    std::vector< std::vector<Point2f> > point_pix_pos;

    int found, successes = 0;
    Size img_size;

    int cnt = 0;
    int k = 0, n = 0;
    for (int i = 0; i < 41; i++){
        //cv::Mat src0 = cv::imread("../calib2/"+std::__cxx11::to_string(i)+"_orig.jpg");
        cv::Mat src0 = cv::imread("../calib2/"+std::__cxx11::to_string(i)+".jpg");
        if ( !cnt ) {
            img_size.width = src0.cols;
            img_size.height = src0.rows;
            //std::cout<<src0.cols<<" "<<src0.rows<<std::endl;
        }
/*
        cv::Mat src_tmp = src0.clone();
        cv::resize(src_tmp, src_tmp, {640, 384});
        cv::imshow("1",src_tmp);
        waitKey();
*/
        found = findChessboardCorners( src0, board_size, point_pix_pos_buf );  //提取角点信息

        //for (auto u:point_pix_pos_buf) std::cout<<u<<std::endl;
        //std::cout<<point_pix_pos_buf.size();

        if ( found && point_pix_pos_buf.size() == board_n ) {
            successes++;
            cvtColor( src0, gray_img, COLOR_BGR2GRAY );
            find4QuadCornerSubpix( gray_img, point_pix_pos_buf, Size( 3, 3 ) );
            point_pix_pos.push_back( point_pix_pos_buf );
            drawn_img = src0.clone();
            drawChessboardCorners( drawn_img, board_size, point_pix_pos_buf, found );
            //imshow( "corners"+std::__cxx11::to_string(i), drawn_img );
            //waitKey( 500 );
        } else
            std::cout << "\tbut failed to found all chess board corners in this image: " << i << std::endl;
        point_pix_pos_buf.clear();
        cnt++;

    };
    std::cout << successes << " useful chess boards" << std::endl;

    Size square_size( 10, 10 );   //size of square  same unit with tvecs; (10mm)
    std::vector< std::vector< Point3f > > point_grid_pos;
    std::vector< Point3f > point_grid_pos_buf;
    std::vector< int > point_count;

    Mat camera_matrix( 3, 3, CV_32FC1, Scalar::all( 0 ) );
    Mat dist_coeffs( 1, 5, CV_32FC1, Scalar::all( 0 ) );  // distortion coeffs
    std::vector< Mat > rvecs;
    std::vector< Mat > tvecs;
    //clac world coordinate of each point; (z=0)
    for (int i = 0; i < successes; i++ ) {
        for (int j = 0; j < board_h; j++ ) {
            for (int k = 0; k < board_w; k++ ){
                Point3f pt;
                pt.x = k * square_size.width;
                pt.y = j * square_size.height;
                pt.z = 0;
                point_grid_pos_buf.push_back( pt );
            }
        }
        point_grid_pos.push_back( point_grid_pos_buf );
        point_grid_pos_buf.clear();
        point_count.push_back( board_h * board_w );
    }

    std::cout << calibrateCamera( point_grid_pos, point_pix_pos, img_size, camera_matrix, dist_coeffs, rvecs, tvecs ) << std::endl;
    std::cout << camera_matrix << std::endl << dist_coeffs << std::endl;

    cv::Mat src = cv::imread("../calib2/1.jpg");
    cv::Mat ret ;
    cv::undistort( src, ret, camera_matrix, dist_coeffs);
    cv::imshow("ret.jpg",ret);
    cv::imwrite("ret.jpg",ret);
    waitKey();
    return 0;
}