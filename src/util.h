#ifndef UTIL_H
#define UTIL_H

#include <opencv2/core/mat.hpp>
#include <string>

namespace inspector {
cv::Mat plot_hist(std::string&& title,cv::Mat& image);
  std::size_t check_level(cv::Mat& image);
} // namespace inspector
#endif /* UTIL_H */
