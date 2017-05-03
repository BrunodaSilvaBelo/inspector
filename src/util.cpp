#include "util.h"
#include <opencv2/core/fast_math.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

namespace inspector {
cv::Mat plot_hist(std::string&& title, cv::Mat& image) {
  cv::Mat hsv;
  cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
  cv::Mat hist;
  int nimages           = 1;
  int channels[]        = {0};
  int dims              = 1;
  int hist_size[]       = {180};
  float hranges[]       = {0, 180};
  float const* ranges[] = {hranges};

  cv::calcHist(&hsv, nimages, channels, cv::Mat(), hist, dims, hist_size,
               ranges, true);

  int hist_w = 512, hist_h = 400;
  int bin_w = cvRound((double)hist_w / 180);
  cv::Mat hist_image(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
  cv::normalize(hist, hist, 0, hist_image.rows, cv::NORM_MINMAX, -1, cv::Mat());

  for (int i = 0; i < hist_size[0]; ++i)
    cv::line(hist_image, cv::Point(bin_w * (i - 1),
                                   hist_h - cvRound(hist.at<float>(i - 1))),
             cv::Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
             cv::Scalar(255, 255, 255), 2, 8, 0);

  // int hist_bins = 50, sat_bins = 60;
  // int const hist_size[]    = {hist_bins, sat_bins};
  // float const sat_range[]  = {0, 256};
  // float const hist_range[] = {0, 180};
  // float const* ranges[]    = {hist_range, sat_range};

  // cv::MatND hist;
  // int channels[] = {0};
  // cv::calcHist(&image, 1, channels, cv::Mat(), hist, 2, hist_size, ranges,
  // true,
  //              false);
  // //cv::normalize(hist, hist, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

  // double maxVal = 0;
  // cv::minMaxLoc(hist, 0, &maxVal, 0, 0);

  // int const scale = 10;
  // cv::Mat hist_image =
  //     cv::Mat::zeros(sat_bins * scale, hist_bins * 10, CV_8UC3);

  // for (std::size_t h = 0; h < hist_bins; ++h) {
  //   for (std::size_t s = 0; s < sat_bins; ++s) {
  //     auto binVal    = hist.at<float>(h, s);
  //     auto intensity = cvRound(binVal * 255 / maxVal);
  //     cv::rectangle(hist_image, cv::Point(h * scale, s * scale),
  //                   cv::Point((h + 1) * scale - 1, (s + 1) * scale - 1),
  //                   cv::Scalar::all(intensity), CV_FILLED);
  //   }
  // }
  cv::namedWindow(title, 0);
  cv::imshow(std::move(title), hist_image);
  return hist;
}

std::size_t check_level(cv::Mat& image) {
  cv::Mat bw;
  cv::cvtColor(image, bw, cv::COLOR_BGR2GRAY);
  cv::threshold(bw, bw, 60, 255, cv::THRESH_OTSU);
  cv::imshow("Gray", bw);
  std::size_t const x = 70;
  std::size_t y       = 10;
  for (; y < bw.rows; ++y) {
    if (bw.at<int>(y, x) == 0)
      break;
  }
  return y;
}
} // namespace inspector
