#include <opencv2/opencv.hpp>
#include "seg_annotator.hpp"
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

SegAnnotator::SegAnnotator(vector<String> img_paths):img_paths(img_paths){
    int n_imgs = img_paths.size();
    deltas.resize(n_imgs);
    cur_idx = 0;
    drawing = false;
}
void SegAnnotator::seg_callback(int  event, int  x, int  y, int  flag, void *param){
    SegAnnotator* segA_ptr = (SegAnnotator*)param;
    int cur_idx = segA_ptr->cur_idx;
    if (event == EVENT_LBUTTONDOWN) {
        segA_ptr->temp_mask.setTo((0));
        segA_ptr->drawing = true;
        Point center = Point(x,y);
        circle(segA_ptr->temp_mask,center,5,(1),5);
        segA_ptr->temp_img.setTo(Scalar(255,0,0), segA_ptr->temp_mask>0);
        imshow("Display Image", segA_ptr->temp_img);
    }
    else if(event == EVENT_MOUSEMOVE){
        if (segA_ptr->drawing){
            Point center = Point(x,y);
            circle(segA_ptr->temp_mask,center,5,(1),5);
            segA_ptr->temp_img.setTo(Scalar(255,0,0), segA_ptr->temp_mask>0);
            imshow("Display Image", segA_ptr->temp_img);
        }
    }
    else if(event == EVENT_LBUTTONUP){
        segA_ptr->drawing = false;
        if(countNonZero(segA_ptr->temp_mask>segA_ptr->total_mask)>0){
            SparseMat smat = SparseMat(segA_ptr->temp_mask>segA_ptr->total_mask);
            segA_ptr->total_mask.setTo((1), segA_ptr->temp_mask>segA_ptr->total_mask);
            segA_ptr->deltas[cur_idx].push_back(smat);
        }
    }
    else if(event == EVENT_RBUTTONUP){
        if (segA_ptr->deltas[cur_idx].size()>0){
            SparseMat top_sparse = segA_ptr->deltas[cur_idx].back();
            Mat top_draw;
            top_sparse.convertTo(top_draw, -1);
            segA_ptr->total_mask -= top_draw;
            segA_ptr->deltas[cur_idx].pop_back();
            segA_ptr->cur_img.copyTo(segA_ptr->temp_img, segA_ptr->total_mask==0);
            imshow("Display Image", segA_ptr->temp_img);
        }
    }
    return;
}