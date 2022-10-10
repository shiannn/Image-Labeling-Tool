#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <stack>
#include <fstream>
#include "BBox_Annotator/bbox_annotator.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv )
{   
    cout << "OpenCV version : " << CV_VERSION << endl;
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }
    vector<String> image_paths;
    glob(argv[1], image_paths, false);
    MouseCapture mcap(image_paths);
    while(true){
        int i = mcap.cur_idx;
        Mat image;
        //image = imread( argv[1], 1 );
        image = imread( image_paths[i], 1 );
        if ( !image.data )
        {
            printf("No image data \n");
            return -1;
        }
        image.copyTo(mcap.cur_imgs[i]);
        mcap.draw_points(image);
        namedWindow("Display Image", WINDOW_AUTOSIZE );
        setMouseCallback("Display Image", mcap.mouse_callback, &mcap);

        imshow("Display Image", image);
        int key = waitKey(0);
        if (char(key) == 'q'){
            mcap.write2csv();
            break;
        }
        else if (char(key) == 'a'){
            mcap.cur_idx = (mcap.cur_idx-1)%image_paths.size();
        }
        else if (char(key) == 'd'){
            mcap.cur_idx = (mcap.cur_idx+1)%image_paths.size();
        }
    }
    return 0;
}