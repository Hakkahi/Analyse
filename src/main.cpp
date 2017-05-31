#include "opencv2/opencv.hpp"
#include "opencv2/bgsegm.hpp"
#include "opencv2/video.hpp"
#include <iostream>
#include <vector>
#include <tuple>
#include <string>

#include "utils.hpp"


using namespace cv;

int main(int argc, char ** argv){

  VideoCapture cap(1);

  double fps = cap.get(CV_CAP_PROP_FPS);
  int wait = 1000 / fps;

  if ( !cap.isOpened())
  {
    std::cout << "Cannot open the video file" << std::endl;
    return EXIT_FAILURE;
  }

  Mat frame;
  Mat frameWithoudFoground;
  Mat mogThreshold;


  Ptr<BackgroundSubtractor> mog = bgsegm::createBackgroundSubtractorMOG();

  backgroundLearning(cap, mog, 300);
  std::vector<std::tuple<Scalar, Scalar, Scalar, std::string>> colorsToDetect;
  //red - right hand
  colorsToDetect.push_back(std::make_tuple(Scalar(150, 70, 150), Scalar(180, 255, 255), Scalar(0, 0, 255), "red"));
  colorsToDetect.push_back(std::make_tuple(Scalar(0, 130, 150), Scalar(10, 255, 255), Scalar(0, 0, 255), "red"));

  //blue
  //colorsToDetect.push_back(std::make_tuple(Scalar(90, 90, 50), Scalar(130, 255, 255), Scalar(255, 0, 0), "blue"));
  //green
  //colorsToDetect.push_back(std::make_tuple(Scalar(42, 50, 50), Scalar(89, 255, 240), Scalar(0, 255, 0), "green"));
  //yellow - left hand
  colorsToDetect.push_back(std::make_tuple(Scalar(15, 100, 70), Scalar(40, 255, 255), Scalar(0, 255, 255), "yellow"));
  initutils(colorsToDetect.size()-1);

  while(1)
  {
    bool bSuccess = cap.read(frame);

    if (!bSuccess)
    {
      std::cout << "Cannot read the frame from video file" << std::endl;
      break;
    }


    backgroundSubtractor(frame, mogThreshold, mog);

    frame.copyTo(frameWithoudFoground, mogThreshold);
    Mat colorFrame = Mat::zeros( frame.size(), CV_8UC3 );
    int idd = 0;
    std::string ids = "red";
    for (size_t i = 0; i < colorsToDetect.size(); i++) {
      if(std::get<3>(colorsToDetect[i]) != ids) idd++;
      colorFrame += detectColor(idd, frameWithoudFoground, std::get<0>(colorsToDetect[i]), std::get<1>(colorsToDetect[i]), std::get<2>(colorsToDetect[i]), std::get<3>(colorsToDetect[i]));
    }
    //imshow("frame", frame);
    imshow("frameWithoudFoground", frameWithoudFoground);
    //imshow("mog2Frame", mogThreshold);
    imshow("colorFrame", colorFrame);

    frameWithoudFoground = Mat();

    if(waitKey(wait) == 27)
    {
      break;
    }
  }
  return EXIT_SUCCESS;
}
