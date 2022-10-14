#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <stack>
#include <fstream>
#include "BBox_Annotator/bbox_annotator.hpp"
#include "Seg_Annotator/seg_annotator.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv ){
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }
    vector<String> image_paths;
    glob(argv[1], image_paths, false);
    SegAnnotator seg_annotator(image_paths);
    while(true){
        int i = seg_annotator.cur_idx;
        Mat image;
        //image = imread( argv[1], 1 );
        image = imread( image_paths[i], 1 );
        if ( !image.data )
        {
            printf("No image data \n");
            return -1;
        }
        seg_annotator.total_mask = Mat(image.size(), CV_8UC1, Scalar(0));
        seg_annotator.temp_mask = Mat(image.size(), CV_8UC1, Scalar(0));
        image.copyTo(seg_annotator.temp_img);
        image.copyTo(seg_annotator.cur_imgs[i]);
        image.copyTo(seg_annotator.pre_imgs[i]);
        // TODO: need to draw the mask results here
        namedWindow("Display Image", WINDOW_AUTOSIZE );
        setMouseCallback("Display Image", seg_annotator.seg_callback, &seg_annotator);

        imshow("Display Image", image);
        int key = waitKey(0);
        if (char(key) == 'q'){
            break;
        }
        else if (char(key) == 'a'){
            seg_annotator.cur_idx = (seg_annotator.cur_idx-1)%image_paths.size();
        }
        else if (char(key) == 'd'){
            seg_annotator.cur_idx = (seg_annotator.cur_idx+1)%image_paths.size();
        }
    }
    return 0;
}
int main1(int argc, char** argv )
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