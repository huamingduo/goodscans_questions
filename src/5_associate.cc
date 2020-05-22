#include <iostream>
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Eigen>

using Transform2D = Eigen::Transform<float, 2, Eigen::Affine>;

void Associate(const cv::Mat& image0, const cv::Mat& image1, Transform2D& transform) {
  if (image0.size() != image1.size()) {
    std::cout << "image size not equal" << std::endl;
    return;
  }

  std::vector<Eigen::Vector2f> points0, points1;
  // find keypoints
  for (int i=0; i<image0.rows; ++i) {
    for (int j=0; j<image0.cols; ++j) {
      if (image0.at<uchar>(i,j) == 1) {
        points0.push_back(Eigen::Vector2f(i,j));
      }
      if (image1.at<uchar>(i,j) == 1) {
        points1.push_back(Eigen::Vector2f(i,j));
      }
    }
  }

  if (points0.empty() || points1.empty()) {
    std::cout << "no keypoints found in one or both the image(s)" << std::endl;
    return;
  }

  double prev_error{0};
  for (int iter=0; iter<10; ++iter) {
    // compute transformed points
    std::vector<Eigen::Vector2f> transformed_points0;
    for (Eigen::Vector2f point : points0) {
      transformed_points0.push_back(transform * point);
    }

    // find correspondence
    std::map<int, int> pairs;
    for (int i=0; i<transformed_points0.size(); ++i) {
      double min_dist{1e3};
      for (int j=0; j<points1.size(); ++j) {
        double dist = (points1[j] - transformed_points0[i]).norm();
        if (dist > min_dist) {
          continue;
        }
        min_dist = dist;
        pairs[i] = j;
      }
    }

    // compute center of mass
    Eigen::Vector2f center0{Eigen::Vector2f::Zero()};
    Eigen::Vector2f center1{Eigen::Vector2f::Zero()};
    for (int i=0; i<points0.size(); ++i) {
      center0 += points0[i];
      center1 += points1[pairs[i]];
    }
    center0 /= points0.size();
    center1 /= points0.size();

    // remove center of mass from point clouds
    std::vector<Eigen::Vector2f> centered_points0, centered_points1;
    for (int i=0; i<points0.size(); ++i) {
      centered_points0.push_back(points0[i] - center0);
      centered_points1.push_back(points1[pairs[i]] - center1);
    }

    // compute R and t using SVD method
    Eigen::Matrix2f W{Eigen::Matrix2f::Identity()};
    for (int i=0; i<centered_points0.size(); ++i) {
      W += centered_points0[i] * centered_points1[i].transpose();
    }
    Eigen::JacobiSVD<Eigen::Matrix2f> svd(W, Eigen::ComputeFullU | Eigen::ComputeFullV);
    Eigen::Matrix2f U = svd.matrixU();
    Eigen::Matrix2f V = svd.matrixV();
    if (U.determinant() * V.determinant() < 0) {
      for (int x = 0; x < 2; ++x) {
        U(x, 1) *= -1;
      }
    }
    Eigen::Matrix2f R = U * V.transpose();
    Eigen::Vector2f t = center1 - R * center0;
    std::cout << "R: " << std::endl << R << std::endl;
    std::cout << "t: " << t.transpose() << std::endl;
    transform = Transform2D::Identity();
    transform.translate(t);
    transform.rotate(R);

    // compute matching error
    double error{0};
    for (int i=0; i<points0.size(); ++i) {
      error += (points1[pairs[i]] - transform * points0[i]).norm();
    }
    std::cout << "E: " << error << std::endl;
    if (std::abs(prev_error - error)/points0.size() < 0.001) {
      std::cout << std::endl << "convergent at iteration " << iter << std::endl << std::endl;
      break;
    }
    prev_error = error;
  }
}

int main(int argc, char** argv) {
  cv::Mat image0 = cv::Mat::zeros(60, 60, CV_8U);
  cv::Mat image1 = cv::Mat::zeros(60, 60, CV_8U);
  for (int i=10; i<60; i += 10) {
    for (int j=10; j<60; j += 15) {
      image0.at<uchar>(i,j) = 1;
      image1.at<uchar>(i+3, j+3) = 1;
    }
  }
  Transform2D transform{Transform2D::Identity()};
  Associate(image0, image1, transform);

  cv::namedWindow("result", CV_WINDOW_NORMAL);
  cv::imshow("result", 255*image0);
  cv::waitKey(0);
  cv::imshow("result", 255*image1);
  cv::waitKey(0);
  return 0;
}

