#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int bw_threshold = 230;

Mat& turnToBW(Mat& bw){

    for( int y = 0; y < bw.rows; y++ )
    { 
        for( int x = 0; x < bw.cols; x++ )
        { 
            for( int c = 0; c < 3; c++ )
            { 
                if ( bw.at<uchar>(y,x) > bw_threshold ){
                    bw.at<uchar>(y,x) = 255;
                }else{
                    bw.at<uchar>(y,x) = 0;
                }
            }
        }
    }
    return bw;
}

Mat& fillAuxMatrix(Mat& aux){
    
}

int etiquetarImagen(Mat& bw)
{
    //Bitmap pic = (Bitmap)pb_copia.Image;
    int width = bw.rows;
    int height = bw.cols;
    int etiquetas[width][height];
    int aux[width][height];
    int etiquetada = 0;
    int label = 0;
    int objetos = 0;

    for (int j = 0; j < height; j++) //lleno matriz auxiliar
    {
        for (int i = 0; i < width; i++)
        {
            if ( bw.at<uchar>(j,i) == 0 ){
                aux[i][j] = 1;
            }
            else{
                aux[i][j] = 0;
            }
        }
    }
            
    for (int i = 0; i<width; i++)
    {
        if (aux[ i ][ 0 ] == 1)  //primer columna
        {
            if (i>0 && aux[i-1][0] != 0)
                etiquetas[i][ 0] = etiquetas[i-1][0];
            else if(i>0 && aux[i-1][1] != 0)
                etiquetas[i][0] = etiquetas[i-1][1];
            else
                etiquetas[i][0] = ++label;
        
            if (i>0 && etiquetas[i-1][1] != 0 && etiquetas[i][0] != etiquetas[i-1][1])
            {
                for(int k = 0; k <= i; k++)
                    for(int l = 0; l < height; l++)
                    {
                        if (etiquetas[i][0] < etiquetas[i-1][1] && etiquetas[k][l] == etiquetas[i-1][1])
                            etiquetas[k][l] = etiquetas[i][0];
                        if (etiquetas[i][0] > etiquetas[i-1][1] && etiquetas[k][l] == etiquetas[i][0])
                            etiquetas[k][l] = etiquetas[i-1][1];
                    }
            }
        }
        
        else
            etiquetas[i][0] = 0;
            
        for (int j = 1; j<height; j++)
        {
            if (aux[i][j] == 1)
            {
                if (i>0 && aux[i-1][j] != 0)
                    etiquetas[i][j] = etiquetas[i-1][j];
                else if (aux[i][j-1] != 0)
                    etiquetas[i][j] = etiquetas[i][j-1];
                else if (i>0 && aux[i-1][j-1] != 0)
                    etiquetas[i][j] = etiquetas[i-1][j-1];
                else if(i>0 && j<(height-1)&& aux[i-1][j+1] != 0)
                    etiquetas[i][j] = etiquetas[i-1][j+1];
                else
                    etiquetas[i][j] = ++label;
        
                if (etiquetas[i][j-1] != 0 && etiquetas[i][j] != etiquetas[i][j-1])
                    for (int k = 0; k <= i; k++)
                        for (int l = 0; l < height; l++)
                        {
                            if (etiquetas[i][j] < etiquetas[i][j-1] && etiquetas[k][l] == etiquetas[i][j-1])
                                etiquetas[k][l] = etiquetas[i][j];
                            if (etiquetas[i][j] > etiquetas[i][j-1] && etiquetas[k][l] == etiquetas[i][j])
                                etiquetas[k][l] = etiquetas[i][j-1];
                        }
                    
                if (i>0 && etiquetas[i-1][j-1] != 0 && etiquetas[i][j] != etiquetas[i-1][j-1])
                    for (int k = 0; k <= i; k++)
                        for (int l = 0; l < height; l++)
                        {
                            if (etiquetas[i][j] < etiquetas[i-1][j-1] && etiquetas[k][l] == etiquetas[i-1][j-1])
                                etiquetas[k][l] = etiquetas[i][j];
                            if (etiquetas[i][j] > etiquetas[i-1][j-1] && etiquetas[k][l] == etiquetas[i][j])
                                etiquetas[k][l] = etiquetas[i-1][j-1];
                        }
        
                if (i > 0 && j < (height - 1) && etiquetas[i - 1][j + 1] != 0 && etiquetas[i][j] != etiquetas[i - 1][j + 1])
                    for (int k = 0; k <= i; k++)
                        for (int l = 0; l < height; l++)
                        {
                            if (etiquetas[i][j] < etiquetas[i - 1][j + 1] && etiquetas[k][l] == etiquetas[i - 1][j + 1])
                                etiquetas[k][l] = etiquetas[i][j];
                            if (etiquetas[i][ j] > etiquetas[i - 1][j + 1] && etiquetas[k][l] == etiquetas[i][j])
                                etiquetas[k][l] = etiquetas[i - 1][j + 1];
                        }

            }
            else
                etiquetas[i][j] = 0;
        }
    }
        
    int x[label+1];
    objetos = 0;

    for (int i = 0; i<width; i++)
        for (int j = 0; j<height; j++)
            if (etiquetas[i][j] != 0)
            {
                //printf(" test \n");
                int n = 0;
                for (int k = 0; k<=objetos; k++)
                    if (etiquetas[i][j] == x[k])
                        n = 1;
                if (n != 1)
                    x[objetos++] = etiquetas[i][j];
            }
        
    for (int i = 0; i<objetos; i++)
    {
        for(int k = 0; k<width; k++)
            for (int l = 0; l<height; l++)
                if (etiquetas[k][l]  == x[i])
                    etiquetas[k][l] = i+1;
    }
        
    etiquetada = 1;
    return objetos ;

        
}

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage2: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image;
    image = imread( argv[1], 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    //create gray image
    Mat gray_image;
    cvtColor( image, gray_image, CV_BGR2GRAY );
    //Get a black and white
    gray_image = turnToBW(gray_image);

    int objects = etiquetarImagen(gray_image);

    printf("Objects = %d \n",objects);

    namedWindow("Display Image", CV_WINDOW_AUTOSIZE );
    printf("Displaying another image copy\n");
    imshow("Display Image", gray_image);

    waitKey(0);

    return 0;
}
