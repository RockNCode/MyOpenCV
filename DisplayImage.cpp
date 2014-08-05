#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv )
{


    if ( argc != 2 )
    {
        printf("usage2: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image, imagecopy;

    image = imread( argv[1], 1 );

    Mat anotherImageCopy(image,Rect(0, 0, 20, 20));

    imagecopy=image(Range::all(), Range(1,30));

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Display Image", CV_WINDOW_AUTOSIZE );
    printf("Displaying another image copy\n");
    imshow("Display Image", anotherImageCopy);

    cout << "Image = "<< endl << " "  << anotherImageCopy << endl << endl;

    waitKey(0);

    return 0;
}
