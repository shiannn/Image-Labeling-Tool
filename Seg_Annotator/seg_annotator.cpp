#include <opencv2/opencv.hpp>
#include "seg_annotator.hpp"

using namespace cv;
using namespace std;

// class SegAnnotator{
//     public:
//         vector<Mat> cur_imgs;
//         vector<String> img_paths;
//         int cur_idx;
// }
SegAnnotator::SegAnnotator(vector<String> img_paths):img_paths(img_paths){
    int n_imgs = img_paths.size();
    cur_imgs.resize(n_imgs);
    cur_idx = 0;
    drawing = false;
}
void SegAnnotator::seg_callback(int  event, int  x, int  y, int  flag, void *param){
    SegAnnotator* segA_ptr = (SegAnnotator*)param;
    int cur_idx = segA_ptr->cur_idx;
    Mat temp_img(segA_ptr->cur_imgs[cur_idx]);
    Mat mask(segA_ptr->cur_imgs[cur_idx].size(), CV_8UC1);
    if (event == EVENT_LBUTTONDOWN) {
        segA_ptr->drawing = true;
    }
    else if(event == EVENT_MOUSEMOVE){
        if (segA_ptr->drawing){
            //cout << "(" << x << ", " << y << ")" << endl;
            mask.at<double>(x,y,0) = 1;
            cout << countNonZero(mask) <<endl;
            Point center = Point(x,y);
            circle(temp_img,center,5,(0,0,255),5);
            imshow("Display Image", temp_img);
        }
    }
    else if(event == EVENT_LBUTTONUP){
        segA_ptr->drawing = false;
    }
    return;
}