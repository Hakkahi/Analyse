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
void backgroundSubtractor(Mat& frame, Mat& mogThreshold, Ptr<BackgroundSubtractor>& mog){

  mog->apply(frame, mogThresold, 0);

  erode(mogThresold, mogThresold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
  dilate(mogThresold, mogThresold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

  dilate(mogThresold, mogThresold, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
  erode(mogThresold, mogThresold, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
}

Mat detectColor(Mat& frame, Scalar lowerHSV, Scalar upperHSV, Scalar color){

  Mat threshold;
  Mat imgHSV;
  Mat imgArea = Mat::zeros(frame.size(), CV_8UC3);
  std::vector<std::vector<Point> > contours;
  std::vector<Vec4i> hierarchy;

  // Conversion de l'image en RGB en image en BVH
  cvtColor(frame, imgHSV, COLOR_BGR2HSV);

  // Création du threshold de l'image en fonction du seuil donné en paramètre
  inRange(imgHSV, lowerHSV, upperHSV, threshold);

  //imshow("HSV", imgHSV);

  // Ouverture pour éliminer les petite imperfections de l'image binaire
  erode(threshold, threshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( threshold, threshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  // Fermeture Pour remplir les petits trous dans l'image binaire
  dilate( threshold, threshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  erode(threshold, threshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

  findContours(threshold, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
  for( int i = 0; i< contours.size(); i++ )
   {
     drawContours(imgArea, contours, i, color, -1, 4, hierarchy, 0, Point());
   }

   return imgArea;
}
