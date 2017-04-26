#include "opencv2/opencv.hpp"
#include "opencv2/bgsegm.hpp"
#include "opencv2/video.hpp"
#include "opencv2/xphoto/white_balance.hpp"
#include <iostream>
#include <string>

#include "utils.hpp"


using namespace cv;

int main(int argc, char ** argv){

  VideoCapture cap(0);

  double fps = cap.get(CV_CAP_PROP_FPS);
  int wait = 1000 / fps;

  if ( !cap.isOpened())
  {
    std::cout << "Cannot open the video file" << std::endl;
    return EXIT_FAILURE;
  }

  Mat frame;
  Mat forgroundFrame;
  Mat mogFrame;


  Ptr<BackgroundSubtractor> mog = bgsegm::createBackgroundSubtractorMOG();

  backgroundLearning(cap, mog, 300);

  while(1)
  {
    bool bSuccess = cap.read(frame);

    if (!bSuccess)
    {
      std::cout << "Cannot read the frame from video file" << std::endl;
      break;
    }

    backgroundSubtractor(frame, mogFrame, mog);

    frame.copyTo(forgroundFrame, mogFrame);

    //imshow("frame", frame);
    imshow("forgroundFrame", forgroundFrame);
    imshow("mog2Frame", mogFrame);

    forgroundFrame = Mat();

    if(waitKey(wait) == 27)
    {
      break;
    }
  }
  return EXIT_SUCCESS;
}
