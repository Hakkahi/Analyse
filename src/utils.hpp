#ifndef _UTILS_H
#define _UTILS_H

#include "opencv2/opencv.hpp"
#include "opencv2/bgsegm.hpp"
#include "opencv2/video.hpp"
#include "opencv2/xphoto/white_balance.hpp"
#include <iostream>
#include <vector>

using namespace cv;

void backgroundLearning(VideoCapture& cap, Ptr<BackgroundSubtractor>& mog, int nbFrame);
void backgroundSubtractor(Mat& frame, Mat& mogThresold, Ptr<BackgroundSubtractor>& mog);

#endif
