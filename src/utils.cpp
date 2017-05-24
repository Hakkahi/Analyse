#include "utils.hpp"

class timer
{
    // alias our types for simplicity
    using clock             = std::chrono::system_clock;
    using time_point_type   = std::chrono::time_point < clock, std::chrono::milliseconds > ;
public:
    // default constructor that stores the start time
    timer()
    {
        start = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
    }

    // gets the time elapsed from construction.
    long /*milliseconds*/ getTimePassed()
    {
        // get the new time
        auto end = clock::now();

        // return the difference of the times
        return (end - start).count()/1000000;
    }
    void restart(){
      start = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
    }
private:
    time_point_type start;
};

std::vector<std::pair<int, int>> iLasts;
std::vector<timer> timers;

void initutils(int v){
  for (int i = 0; i < v; ++i)
  {
    iLasts.push_back(std::make_pair(-1, -1));
  timer t;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "passed : " << t.getTimePassed() << std::endl;
  timers.push_back(t);
  }
  
}

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

  std::cout << "Fond appris, vous pouvez y aller !" << std::endl;
}


/**
 * Extract the moving element of the @frame (colored picture)
 * and generate a Threshold (in @mogThreshold) with only the moving element
 * @mog trained statistical backgroundSubtractor
 */
void backgroundSubtractor(Mat& frame, Mat& mogThreshold, Ptr<BackgroundSubtractor>& mog){

  mog->apply(frame, mogThreshold, 0);

  erode(mogThreshold, mogThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
  dilate(mogThreshold, mogThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

  dilate(mogThreshold, mogThreshold, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
  erode(mogThreshold, mogThreshold, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
}

Mat detectColor(int id, Mat& frame, Scalar lowerHSV, Scalar upperHSV, Scalar color, std::string message){

  Mat threshold;
  Mat imgHSV;
  Mat imgArea = Mat::zeros(frame.size(), CV_8UC3);
  std::vector<std::vector<Point> > contours;
  std::vector<Vec4i> hierarchy;


  // Conversion de l'image en RGB en image en BVH
  cvtColor(frame, imgHSV, COLOR_BGR2HSV);

  // Création du threshold de l'image en fonction du seuil donné en paramètre
  inRange(imgHSV, lowerHSV, upperHSV, threshold);

  // Ouverture pour éliminer les petite imperfections de l'image binaire
  erode(threshold, threshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( threshold, threshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  // Fermeture Pour remplir les petits trous dans l'image binaire
  dilate( threshold, threshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  erode(threshold, threshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

  Moments oMoments = moments(threshold);
  double dM01 = oMoments.m01;
  double dM10 = oMoments.m10;
  double dArea = oMoments.m00;

  // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero
  if (dArea > 14000)
  {
   //calculate the position of the ball
   int posX = dM10 / dArea;
   int posY = dM01 / dArea;
   //std::cout << "IN: "<< std::endl;
   if (iLasts[id].first >= 0 && iLasts[id].second >= 0 && posX >= 0 && posY >= 0)
   {
    //Draw a red line from the previous point to the current point
    //line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);
    //circle(imgArea, Point(posX, posY), 10, Scalar(0,0,255), -1);
      double res = norm(Point(iLasts[id].first, iLasts[id].second) - Point(posX, posY));
      //std::cout << "DISTANCE : " << res << std::endl;
      
      if(res >= 40){
        //time_t ttime;
        //time(&ttime);
        long difft = timers[id].getTimePassed();
        std::cout << "MOUVEMENT MOTHERFUCKER !!!!!!!  " << message << ", in " << difft << std::endl;
        Vec2i tor = Point(posX, posY) - Point(iLasts[id].first, iLasts[id].second);
        if (difft > 20 && difft < 1200)
        {
          std::cout << "vector : " << tor << std::endl;
        }
        //timers[id] = ttime;
        
      }
      timers[id].restart();
   }
   iLasts[id].first = posX;
   iLasts[id].second = posY;
  }
  findContours(threshold, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
  for( int i = 0; i< contours.size(); i++ )
   {
     drawContours(imgArea, contours, i, color, -1, 4, hierarchy, 0, Point());
   }

   return imgArea;
}



