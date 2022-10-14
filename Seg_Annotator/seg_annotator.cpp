#include <opencv2/opencv.hpp>
#include "seg_annotator.hpp"
#include <opencv2/core/core.hpp>

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
    pre_imgs.resize(n_imgs);
    deltas.resize(n_imgs);
    cur_idx = 0;
    drawing = false;
}
void SegAnnotator::seg_callback(int  event, int  x, int  y, int  flag, void *param){
    SegAnnotator* segA_ptr = (SegAnnotator*)param;
    int cur_idx = segA_ptr->cur_idx;
    //Mat temp_img(segA_ptr->cur_imgs[cur_idx]);
    //Mat mask(segA_ptr->cur_imgs[cur_idx].size(), CV_8UC1);
    if (event == EVENT_LBUTTONDOWN) {
        segA_ptr->drawing = true;
    }
    else if(event == EVENT_MOUSEMOVE){
        if (segA_ptr->drawing){
            //cout << "(" << x << ", " << y << ")" << endl;
            //mask.at<double>(x,y,0) = 1;
            //cout << countNonZero(mask) <<endl;
            Point center = Point(x,y);
            circle(segA_ptr->temp_img,center,5,(0,0,255),5);
            imshow("Display Image", segA_ptr->temp_img);
        }
    }
    else if(event == EVENT_LBUTTONUP){
        segA_ptr->drawing = false;
        //vector<Mat> channels;
        //split(segA_ptr->temp_img-segA_ptr->pre_imgs[cur_idx], channels);
        //cout << channels[0].size() << endl;
        //vector<Point> delta;
        //findNonZero(channels[0]|channels[1]|channels[2], delta);
        segA_ptr->deltas[cur_idx].push_back(segA_ptr->temp_img.clone());
        // for (auto p:delta){
        //     cout << p << endl;
        // }
        //segA_ptr->temp_img.copyTo(segA_ptr->cur_imgs[cur_idx]);
    }
    else if(event == EVENT_RBUTTONUP){
        if (segA_ptr->deltas[cur_idx].size()>0){
            segA_ptr->deltas[cur_idx].pop_back();
            //cout << segA_ptr->deltas[cur_idx].size() << endl;
            //segA_ptr->draw_mask();
            if (segA_ptr->deltas[cur_idx].size() > 0){
                segA_ptr->deltas[cur_idx].back().copyTo(segA_ptr->temp_img);
            }else{
                segA_ptr->pre_imgs[cur_idx].copyTo(segA_ptr->temp_img);
            }
            imshow("Display Image", segA_ptr->temp_img);
        }
    }
    return;
}

// void SegAnnotator::draw_mask(){
//     // initialize temp_img
//     pre_imgs[cur_idx].copyTo(temp_img);
//     for (int i=0; i<deltas[cur_idx].size();i++){
//         for(int j=0; j<deltas[cur_idx][i].size(); j++){
//             // int x = deltas[cur_idx][i][j].x;
//             // int y = deltas[cur_idx][i][j].y;
//             temp_img.at<Vec3b>(deltas[cur_idx][i][j]) = (0,0,255);
//         }
//     }
// }