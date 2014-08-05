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

double alpha =1; /**< Simple contrast control */
int beta= 5;  /**< Simple brightness control */

double getPSNR ( const Mat& I1, const Mat& I2);
Scalar getMSSIM( const Mat& I1, const Mat& I2);

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
/*
private void etiquetarImagen(Mat& bw)
{
    //Bitmap pic = (Bitmap)pb_copia.Image;
    int width = bw.rows;
    int height = bw.cols;
    int etiquetas[width][height] = {0};
    int aux[width][height] = { 0 };
    int etiquetada = 0;
    int label = 0;
    int objetos = 0;

    for (int j = 0; j < height; j++) //lleno matriz auxiliar
    {
        for (int i = 0; i < width; i++)
        {
            if ( bw.at<uchar>(y,x) == 0 ){
                aux[i][j] = 1;
            }else{
                aux[i][j] = 0;
            }
        }
    }
            
    for (int i = 0; i<width; i++)
    {
        if (aux[ i , 0] == 1)  //primer columna
        {
            if (i>0 && aux[i-1,0] != 0)
                etiquetas[i , 0] = etiquetas[i-1,0];
            else if(i>0 && aux[i-1,1] != 0)
                etiquetas[i,0] = etiquetas[i-1,1];
            else
                etiquetas[i,0] = ++label;
        
            if (i>0 && etiquetas[i-1,1] != 0 && etiquetas[i,0] != etiquetas[i-1,1])
                for(int k = 0; k <= i; k++)
                    for(int l = 0; l < height; l++)
                    {
                        if (etiquetas[i,0] < etiquetas[i-1,1] && etiquetas[k,l] == etiquetas[i-1,1])
                            etiquetas[k,l] = etiquetas[i,0];
                        if (etiquetas[i,0] > etiquetas[i-1,1] && etiquetas[k,l] == etiquetas[i,0])
                            etiquetas[k,l] = etiquetas[i-1,1];
                    }
        }
        
        else
            etiquetas[i,0] = 0;
            
        for (int j = 1; j<height; j++)
            if (aux[i,j] == 1)
            {
                if (i>0 && aux[i-1,j] != 0)
                    etiquetas[i,j] = etiquetas[i-1,j];
                else if (aux[i,j-1] != 0)
                    etiquetas[i,j] = etiquetas[i,j-1];
                else if (i>0 && aux[i-1,j-1] != 0)
                    etiquetas[i,j] = etiquetas[i-1,j-1];
                else if(i>0 && j<(height-1)&& aux[i-1,j+1] != 0)
                    etiquetas[i,j] = etiquetas[i-1,j+1];
                else
                    etiquetas[i,j] = ++label;
        
                if (etiquetas[i,j-1] != 0 && etiquetas[i,j] != etiquetas[i,j-1])
                    for (int k = 0; k <= i; k++)
                        for (int l = 0; l < height; l++)
                        {
                            if (etiquetas[i,j] < etiquetas[i,j-1] && etiquetas[k,l] == etiquetas[i,j-1])
                                etiquetas[k,l] = etiquetas[i,j];
                            if (etiquetas[i,j] > etiquetas[i,j-1] && etiquetas[k,l] == etiquetas[i,j])
                                etiquetas[k,l] = etiquetas[i,j-1];
                        }
                    
                if (i>0 && etiquetas[i-1,j-1] != 0 && etiquetas[i,j] != etiquetas[i-1,j-1])
                    for (int k = 0; k <= i; k++)
                        for (int l = 0; l < height; l++)
                        {
                            if (etiquetas[i,j] < etiquetas[i-1,j-1] && etiquetas[k,l] == etiquetas[i-1,j-1])
                                etiquetas[k,l] = etiquetas[i,j];
                            if (etiquetas[i,j] > etiquetas[i-1,j-1] && etiquetas[k,l] == etiquetas[i,j])
                                etiquetas[k,l] = etiquetas[i-1,j-1];
                        }
        
                if (i > 0 && j < (height - 1) && etiquetas[i - 1,j + 1] != 0 && etiquetas[i,j] != etiquetas[i - 1,j + 1])
                    for (int k = 0; k <= i; k++)
                        for (int l = 0; l < height; l++)
                        {
                            if (etiquetas[i,j] < etiquetas[i - 1,j + 1] && etiquetas[k,l] == etiquetas[i - 1,j + 1])
                                etiquetas[k,l] = etiquetas[i,j];
                            if (etiquetas[i, j] > etiquetas[i - 1,j + 1] && etiquetas[k,l] == etiquetas[i,j])
                                etiquetas[k,l] = etiquetas[i - 1,j + 1];
                        }

            }
            else
                etiquetas[i,j] = 0;
    }
        
    int[] x = new int[label+1];
    objetos = 0;
        
    for (int i = 0; i<width; i++)
        for (int j = 0; j<height; j++)
            if (etiquetas[i,j] != 0)
            {
                int n = 0;
                for (int k = 0; k<=objetos; k++)
                    if (etiquetas[i,j] == x[k])
                        n = 1;
                if (n != 1)
                    x[objetos++] = etiquetas[i,j];
            }
        
    for (int i = 0; i<objetos; i++)
    {
        for(int k = 0; k<width; k++)
            for (int l = 0; l<height; l++)
                if (etiquetas[k,l]  == x[i])
                    etiquetas[k,l] = i+1;
    }
        
    etiquetada = true;
    textBox2.Text = objetos.ToString();
        
         
}
*/
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

    VideoCapture captRefrnc(sourceReference), captUndTst(sourceCompareWith);

    if (!captRefrnc.isOpened())
    {
        cout  << "Could not open reference " << sourceReference << endl;
        return -1;
    }

    if (!captUndTst.isOpened())
    {
        cout  << "Could not open case test " << sourceCompareWith << endl;
        return -1;
    }

    Size refS = Size((int) captRefrnc.get(CV_CAP_PROP_FRAME_WIDTH),
                     (int) captRefrnc.get(CV_CAP_PROP_FRAME_HEIGHT)),
         uTSi = Size((int) captUndTst.get(CV_CAP_PROP_FRAME_WIDTH),
                     (int) captUndTst.get(CV_CAP_PROP_FRAME_HEIGHT));

    if (refS != uTSi)
    {
        cout << "Inputs have different size!!! Closing." << endl;
        return -1;
    }

    const char* WIN_UT = "Under Test";
    const char* WIN_RF = "Reference";

    // Windows
    namedWindow(WIN_RF, CV_WINDOW_AUTOSIZE);
    namedWindow(WIN_UT, CV_WINDOW_AUTOSIZE);
    cvMoveWindow(WIN_RF, 400       , 0);         //750,  2 (bernat =0)
    cvMoveWindow(WIN_UT, refS.width, 0);         //1500, 2

    cout << "Reference frame resolution: Width=" << refS.width << "  Height=" << refS.height
         << " of nr#: " << captRefrnc.get(CV_CAP_PROP_FRAME_COUNT) << endl;

    cout << "PSNR trigger value " << setiosflags(ios::fixed) << setprecision(3)
         << psnrTriggerValue << endl;

    Mat frameReference, frameUnderTest;
    double psnrV;
    Scalar mssimV;

    for(;;) //Show the image captured in the window and repeat
    {
        captRefrnc >> frameReference;
        captUndTst >> frameUnderTest;

        if (frameReference.empty() || frameUnderTest.empty())
        {
            cout << " < < <  Game over!  > > > ";
            break;
        }

        ++frameNum;
        cout << "Frame: " << frameNum << "# ";

        ///////////////////////////////// PSNR ////////////////////////////////////////////////////
        psnrV = getPSNR(frameReference,frameUnderTest);
        cout << setiosflags(ios::fixed) << setprecision(3) << psnrV << "dB";

        //////////////////////////////////// MSSIM /////////////////////////////////////////////////
        if (psnrV < psnrTriggerValue && psnrV)
        {
            mssimV = getMSSIM(frameReference, frameUnderTest);

            cout << " MSSIM: "
                << " R " << setiosflags(ios::fixed) << setprecision(2) << mssimV.val[2] * 100 << "%"
                << " G " << setiosflags(ios::fixed) << setprecision(2) << mssimV.val[1] * 100 << "%"
                << " B " << setiosflags(ios::fixed) << setprecision(2) << mssimV.val[0] * 100 << "%";
        }

        cout << endl;

        ////////////////////////////////// Show Image /////////////////////////////////////////////
        ///Change video frame to grayscale
        Mat gray_image;
        cvtColor( frameReference, gray_image, CV_BGR2GRAY );
        //Mat roi(frameReference);
        //roi = Scalar(0,255,0);
        //Turn gray image into black and white
        //Mat new_image = Mat::zeros( gray_image.size(), gray_image.type() );
        for( int y = 0; y < gray_image.rows; y++ )
        { 
            for( int x = 0; x < gray_image.cols; x++ )
            { 
                //printf("%d \n",y);
                //gray_image.at<Vec3b>(y,x)[1] = 255;
                //gray_image.at<uchar>(y,x) = 255; //white
                for( int c = 0; c < 3; c++ )
                { 
                    if ( gray_image.at<uchar>(y,x) > 100 ){
                        gray_image.at<uchar>(y,x) = 255;
                    }else{
                        gray_image.at<uchar>(y,x) = 0;
                    }
                    //gray_image.at<Vec3b>(y,x)[c] =
                    //    saturate_cast<uchar>( 1 * ( gray_image.at<Vec3b>(y,x)[c] ) + 20 );
                }
            }
        }

        //
        //Tag objects

        //Count objects

        ///////////////////////////////////////////////////////////////////////////////////////////
        imshow(WIN_RF, /*frameReference*/ gray_image);
        //imshow(WIN_UT, frameUnderTest);

        c = (char)cvWaitKey(delay);
        if (c == 27) break;
    }

    return 0;
}

double getPSNR(const Mat& I1, const Mat& I2)
{
    Mat s1;
    absdiff(I1, I2, s1);       // |I1 - I2|
    s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
    s1 = s1.mul(s1);           // |I1 - I2|^2

    Scalar s = sum(s1);        // sum elements per channel

    double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

    if( sse <= 1e-10) // for small values return zero
        return 0;
    else
    {
        double mse  = sse / (double)(I1.channels() * I1.total());
        double psnr = 10.0 * log10((255 * 255) / mse);
        return psnr;
    }
}

Scalar getMSSIM( const Mat& i1, const Mat& i2)
{
    const double C1 = 6.5025, C2 = 58.5225;
    /***************************** INITS **********************************/
    int d = CV_32F;

    Mat I1, I2;
    i1.convertTo(I1, d);            // cannot calculate on one byte large values
    i2.convertTo(I2, d);

    Mat I2_2   = I2.mul(I2);        // I2^2
    Mat I1_2   = I1.mul(I1);        // I1^2
    Mat I1_I2  = I1.mul(I2);        // I1 * I2

    /*************************** END INITS **********************************/

    Mat mu1, mu2;                   // PRELIMINARY COMPUTING
    GaussianBlur(I1, mu1, Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, Size(11, 11), 1.5);

    Mat mu1_2   =   mu1.mul(mu1);
    Mat mu2_2   =   mu2.mul(mu2);
    Mat mu1_mu2 =   mu1.mul(mu2);

    Mat sigma1_2, sigma2_2, sigma12;

    GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;

    GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;

    GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;

    ///////////////////////////////// FORMULA ////////////////////////////////
    Mat t1, t2, t3;

    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);                 // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);                 // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

    Mat ssim_map;
    divide(t3, t1, ssim_map);        // ssim_map =  t3./t1;

    Scalar mssim = mean(ssim_map);   // mssim = average of ssim map
    return mssim;
}
