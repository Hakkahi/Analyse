#include <iostream>
#include "opencv2/core/core.hpp"
//#include "opencv2/bgsegm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <string>
#include <sstream>
 
using namespace cv;
using namespace std;
 
 int main( int argc, char** argv )
 {
 
   VideoCapture vcap(0);
 
      if(!vcap.isOpened()){
             cout << "Error opening video stream or file" << endl;
             return -1;
      }
 
   int frame_width=   vcap.get(CV_CAP_PROP_FRAME_WIDTH);
   int frame_height=   vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
   VideoWriter video("out.avi",CV_FOURCC('M','J','P','G'),vcap.get(CV_CAP_PROP_FPS), Size(frame_width,frame_height),true);
 
  std::cout << "Enregistrement du fond" << std::endl;
 
  Mat frame;

   for(int i = 0; i < 360; ++i)
   {
 
       vcap >> frame;
       video.write(frame);
   }
   
    std::cout << " Vous pouvez y aller les lapins, YOLO " << vcap.get(CV_CAP_PROP_FPS) << std::endl;
 
 
   for(int i = 0; i < 3000; ++i)
    {

        vcap >> frame;
       video.write(frame);

        if(waitKey(33) == 27)
        {
            break;
        }
    }

   return EXIT_SUCCESS;
}