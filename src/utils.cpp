#include "utils.hpp"




/**
 * Learn the background from @cap
 * and train the statistical backgroundSubtractor @mog.
 *
 */
void backgroundLearning(VideoCapture& cap, Ptr<BackgroundSubtractor>& mog, int nbFrame){

  Mat frame;
  Mat mogFrame;

  std::cout << "Apprentissage du fond, ne passez pas devant !" << std::endl;
  for(int i = 0; i < nbFrame; ++i){

    bool bSuccess = cap.read(frame);
    if (!bSuccess)
    {
      std::cout << "Cannot read the frame from video file" << std::endl;
      break;
    }

    mog->apply(frame, mogFrame);
  }

  system("canberra-gtk-play -f Data/beep.wav");

  std::cout << "Fond appris, vous pouvez y aller !" << std::endl;
}


/**
 * Extract the moving element of the @frame (colored picture)
 * and generate a Threshold (in @mogThresold) with only the moving element
 * @mog trained statistical backgroundSubtractor
 */
void backgroundSubtractor(Mat& frame, Mat& mogThresold, Ptr<BackgroundSubtractor>& mog){

  mog->apply(frame, mogThresold, 0);

  erode(mogThresold, mogThresold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
  dilate(mogThresold, mogThresold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

  dilate(mogThresold, mogThresold, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
  erode(mogThresold, mogThresold, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));

  std::vector<std::vector<cv::Point> > contours;

  Mat contourImage(mogThresold.size(), CV_8UC3, cv::Scalar(0,0,0));
  findContours( mogThresold, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
  for (size_t idx = 0; idx < contours.size(); idx++) {

        if(arcLength(contours[idx], true) >= 100)
          drawContours(contourImage, contours, idx, Scalar(255, 0, 0), -1);
    }

  imshow("contourImage", contourImage);
}
