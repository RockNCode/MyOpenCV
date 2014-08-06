#include <iostream> // for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O
#include <stdio.h>

using namespace std;
using namespace cv;

const int bw_slider_max = 255;
int bw_slider;
int bw_threshold = 230;

static void help()
{
    cout
        << "------------------------------------------------------------------------------" << endl
        << "This program shows how to read a video file with OpenCV. In addition, it "
        << "tests the similarity of two input videos first with PSNR, and for the frames "
        << "below a PSNR trigger value, also with MSSIM."                                   << endl
        << "Usage:"                                                                         << endl
        << "./video-source referenceVideo useCaseTestVideo PSNR_Trigger_Value Wait_Between_Frames " << endl
        << "--------------------------------------------------------------------------"     << endl
        << endl;
}

// Trackbar/switch callback
void on_trackbar( int, void* )
{
 bw_threshold = bw_slider ;
 //printf("bw_threshold = %d ",bw_threshold);
}

void on_Button1( int, void* )
{
    //bw_threshold = bw_slider ;
 //printf("bw_threshold = %d ",bw_threshold);
}


int main(int argc, char *argv[])
{
    help();

    if (argc != 5)
    {
        cout << "Not enough parameters" << endl;
        return -1;
    }

    stringstream conv;

    const string sourceReference = argv[1], sourceCompareWith = argv[2];
    int psnrTriggerValue, delay;
    conv << argv[3] << endl << argv[4];       // put in the strings
    conv >> psnrTriggerValue >> delay;        // take out the numbers

    char c;
    int frameNum = -1;          // Frame counter

    VideoCapture captRefrnc(sourceReference);

    if (!captRefrnc.isOpened())
    {
        cout  << "Could not open reference " << sourceReference << endl;
        return -1;
    }

    Size refS = Size((int) captRefrnc.get(CV_CAP_PROP_FRAME_WIDTH),
                     (int) captRefrnc.get(CV_CAP_PROP_FRAME_HEIGHT));

    const char* WIN_RF = "Controls";

    // Windows
    namedWindow(WIN_RF, CV_WINDOW_AUTOSIZE);

    //Create track bar
    char TrackbarName[50];
    sprintf( TrackbarName, "trackbar  %d", bw_slider_max );
    createTrackbar( TrackbarName, WIN_RF, &bw_slider, bw_slider_max, on_trackbar );

    //const char* nameb1 = "button1";
    //createButton(nameb1,on_Button1,nameb1,CV_CHECKBOX,1);

    cout << "Reference frame resolution: Width=" << refS.width << "  Height=" << refS.height
         << " of nr#: " << captRefrnc.get(CV_CAP_PROP_FRAME_COUNT) << endl;

    cout << "PSNR trigger value " << setiosflags(ios::fixed) << setprecision(3)
         << psnrTriggerValue << endl;

    Mat frameReference;

    for(;;) //Show the image captured in the window and repeat
    {
        captRefrnc >> frameReference;


        if (frameReference.empty() )
        {
            cout << " < < <  Game over!  > > > ";
            break;
        }

        ++frameNum;

        //Mat gray_image;
        //cvtColor( frameReference, gray_image, CV_BGR2GRAY );

        for( int y = 0; y < frameReference.rows; y++ )
        { 
            for( int x = 0; x < frameReference.cols; x++ )
            { 

                Vec3b color = frameReference.at<Vec3b>(Point(x,y));
                // ... do something to the color ....
                int ave_rgb = ( color[0] + color [1] + color[2] )/ 3;
                if(ave_rgb > bw_threshold)
                {
                    color[0] = 255;
                    color[1] = 255;
                    color[2] = 255;
                }
                else{
                    color[0] = 0;
                    color[1] = 0;
                    color[2] = 0;
                }

                //color[0] = ave_rgb;
                //color[1] = ave_rgb;
                //color[2] = ave_rgb;

                // set pixel
                frameReference.at<Vec3b>(Point(x,y)) = color;

            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        imshow(TrackbarName, frameReference);

        c = (char)cvWaitKey(delay);
        if (c == 27) break;
    }

    return 0;
}
