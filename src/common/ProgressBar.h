// Copyright 2020 UBC Sailbot

#ifndef COMMON_PROGRESSBAR_H_
#define COMMON_PROGRESSBAR_H_

#include <iostream>
#include <string>

class ProgressBar {
 public:
  /**
   * @param bar_width Width of progress bar
   */
  explicit ProgressBar(uint16_t bar_width = 70) : bar_width_(bar_width), progress_(0) {}

  /**
   * Update progress, should be in [0, 1]
   */
  void update(double progress) { progress_ = progress; }

  /**
   * Print progress bar and follow-up text
   * @param text_after_progress_bar text to be displayed after the progress bar
   */
  void print(const std::string & text_after_progress_bar = "") {
    std::cout << "[";
    int pos = bar_width_ * progress_;
    for (int i = 0; i < bar_width_; ++i) {
        if (i < pos) {
          std::cout << "=";
        } else if (i == pos) {
          std::cout << ">";
        } else {
          std::cout << " ";
        }
    }
    std::cout << "] " << int(progress_ * 100.0) << " %";
    std::cout << text_after_progress_bar << "\r";
    std::cout.flush();
  }

  /**
   * Flush progress bar to allow future prints to work properly
   */
  void flush() { std::cout << std::endl; }

 protected:
  uint16_t bar_width_;
  double progress_;
};

#endif  // COMMON_PROGRESSBAR_H_

