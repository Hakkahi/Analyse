#ifndef _UTILS_H
#define _UTILS_H

#include "opencv2/opencv.hpp"
#include "opencv2/bgsegm.hpp"
#include "opencv2/video.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sstream>
#include <unistd.h>

#define EV_PRESSED 1
#define EV_RELEASED 0
#define EV_REPEAT 2

using namespace cv;

void backgroundLearning(VideoCapture& cap, Ptr<BackgroundSubtractor>& mog, int nbFrame);
void backgroundSubtractor(Mat& frame, Mat& mogThreshold, Ptr<BackgroundSubtractor>& mog);
void triggerAction(int hand, bool top = false, bool right = false);
void initutils(int v);
Mat detectColor(int id, Mat& frame, Scalar lowerHSV, Scalar upperHSV, Scalar color, std::string message="");
void triggerKeyboard();



#endif
