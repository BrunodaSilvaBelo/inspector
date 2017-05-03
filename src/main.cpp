/**
   \mainpage
   \brief
   \author Bruno da Silva Belo
   \see https://github.com/isocpp/CppCoreGuidelines
*/
#include "util.h"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <limits>

/*
  tampa {144,0}, {244,53}
  rotulo {97,170}, {250, 286}

  xxxxx
  first bit - se tem garrafa, 0 para tem
  second bit - se tem tampa
  third bit - liquido normal
  forth bit - rotulo normal
  fifth bit - garrafa normal
 */

int main(int argc, char** argv) {
  if (argc < 2)
    return -1;

  std::vector<std::string> images;
  for (std::size_t i = 2; i < argc; ++i)
    images.push_back(argv[i]);

  std::string s = "CLASS,LIQUIDO,TAMPA_MIN,TAMPA_MAX\n";
  cv::namedWindow("Image", 0);
  cv::namedWindow("Tampa", 0);
  cv::namedWindow("Rotulo", 0);
  cv::namedWindow("Liquido", 0);
  for (auto& i : images) {
    std::string const code = i.substr(25, 5);
    if (code.back() == '1')
      continue;
    int cl = 0;
    for (auto& i : code)
      if (i == '1') {
	cl = 1;
	break;
      }

    s += std::to_string(cl) + ",";
    auto image{cv::imread(i)};
    cv::medianBlur(image, image, 5);
    cv::Rect const tampa_rect(100, 0, 130, 60);
    auto tampa{image(tampa_rect)};

    cv::Rect const rotulo_rect(90, 170, 170, 117);
    auto rotulo{image(rotulo_rect)};

    cv::Rect const liquido_rect(88, 60, 170, 130);
    auto liquido{image(liquido_rect)};
    auto level = inspector::check_level(liquido);
    s += std::to_string(level) + ",";
    decltype(image) hsb;
    cv::cvtColor(rotulo, hsb, cv::COLOR_BGR2HSV);

    auto tampa_hist  = inspector::plot_hist("Tampa hist", tampa);
    auto rotulo_hist = inspector::plot_hist("Rotulo hist", rotulo);
    int min = std::numeric_limits<int>::max(),
        max = std::numeric_limits<int>::min();
    for (int i = 0; i < 30; ++i) {
      auto value = static_cast<int>(tampa_hist.at<uchar>(i));
      min = (min > value) ? value : min;
      max = (max < value) ? value : max;
    }

    s += std::to_string(min) + "," + std::to_string(max);
    cv::imshow("TT", hsb);
    cv::imshow("Liquido", liquido);
    cv::imshow("Rotulo", rotulo);
    cv::imshow("Tampa", tampa);
    cv::imshow("Image", image);
    s += "\n";
    //cv::waitKey(0);
  }

  std::ofstream out(argv[1]);
  out << s;
  out.close();
  return 0;
}
