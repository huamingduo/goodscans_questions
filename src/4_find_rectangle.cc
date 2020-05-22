#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  cv::Mat image = cv::Mat::zeros(80, 80, CV_8U);
  for (int i=10; i<30; ++i) {
    for (int j=10; j<40; ++j) {
      if (i != 10 && j != 10 && i != 29 && j != 39) {
        continue;
      }
      image.at<uchar>(i,j) = 255;
    }
  }
  for (int i=50; i<70; ++i) {
    for (int j=60; j<70; ++j) {
      if (i != 50 && j != 60 && i != 69 && j != 69) {
        continue;
      }
      image.at<uchar>(i,j) = 255;
    }
  }

  int max_area{0};
  int max_rectangle_idx{-1};
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
  for (int i=0; i<contours.size(); ++i) {
    std::vector<cv::Point> simplified;
    cv::approxPolyDP(contours[i], simplified, 0.5, true);
    if (simplified.size() != 4) {
      continue;
    }
    int height = cv::norm(simplified[1] - simplified[0]);
    int width = cv::norm(simplified[2] - simplified[1]);
    if (height * width > max_area) {
      max_rectangle_idx = i;
    }
  }

  cv::drawContours(image, contours, max_rectangle_idx, cv::Scalar(100));
  cv::namedWindow("result", CV_WINDOW_NORMAL);
  cv::imshow("result", image);
  cv::waitKey(0);
  return 0;
}

