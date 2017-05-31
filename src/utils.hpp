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

using namespace cv;

void backgroundLearning(VideoCapture& cap, Ptr<BackgroundSubtractor>& mog, int nbFrame);
void backgroundSubtractor(Mat& frame, Mat& mogThreshold, Ptr<BackgroundSubtractor>& mog);
void triggerAction(int hand, bool top = false, bool right = false);
void initutils(int v);
Mat detectColor(int id, Mat& frame, Scalar lowerHSV, Scalar upperHSV, Scalar color, std::string message="");



#endif
