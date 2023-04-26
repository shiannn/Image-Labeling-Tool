#include <opencv2/opencv.hpp>
#include "seg_annotator.hpp"
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

SegAnnotator::SegAnnotator(vector<String> img_paths):img_paths(img_paths){
    int n_imgs = img_paths.size();
    deltas.resize(n_imgs);
    cur_idx = 0;
    color_idx = 120;
    drawing = false;
    brush_size = 5;
}
void SegAnnotator::seg_callback(int  event, int  x, int  y, int  flag, void *param){
    SegAnnotator* segA_ptr = (SegAnnotator*)param;
    int cur_idx = segA_ptr->cur_idx;
    if (event == EVENT_LBUTTONDOWN) {
        segA_ptr->temp_mask.setTo((0));
        segA_ptr->drawing = true;
        Point center = Point(x,y);
        cout << "use color" << segA_ptr->color_idx << endl;
        circle(segA_ptr->temp_mask,center,segA_ptr->brush_size,(segA_ptr->color_idx),segA_ptr->brush_size);
        Mat img_color;
        applyColorMap(segA_ptr->temp_mask, img_color, COLORMAP_JET);
        img_color.copyTo(segA_ptr->temp_img, segA_ptr->total_mask==0 & segA_ptr->temp_mask>0);
        imshow("Display Image", segA_ptr->temp_img);
    }
    else if(event == EVENT_MOUSEMOVE){
        if (segA_ptr->drawing){
            Point center = Point(x,y);
            circle(segA_ptr->temp_mask,center,segA_ptr->brush_size,(segA_ptr->color_idx),segA_ptr->brush_size);
            Mat img_color;
            applyColorMap(segA_ptr->temp_mask, img_color, COLORMAP_JET);
            img_color.copyTo(segA_ptr->temp_img, segA_ptr->total_mask==0 & segA_ptr->temp_mask>0);
            imshow("Display Image", segA_ptr->temp_img);
        }
    }
    else if(event == EVENT_LBUTTONUP){
        segA_ptr->drawing = false;
        if(countNonZero(segA_ptr->temp_mask>0 & segA_ptr->total_mask==0)>0){
            SparseMat smat = SparseMat(segA_ptr->temp_mask>0 & segA_ptr->total_mask==0);
            segA_ptr->temp_mask.copyTo(
                segA_ptr->total_mask,
                segA_ptr->temp_mask>0 & segA_ptr->total_mask==0
            );
            segA_ptr->deltas[cur_idx].push_back(smat);
        }
    }
    else if(event == EVENT_RBUTTONUP){
        if (segA_ptr->deltas[cur_idx].size()>0){
            SparseMat top_sparse = segA_ptr->deltas[cur_idx].back();
            Mat top_draw;
            top_sparse.convertTo(top_draw, -1);
            segA_ptr->total_mask.setTo(0, top_draw > 0);
            segA_ptr->deltas[cur_idx].pop_back();
            segA_ptr->cur_img.copyTo(segA_ptr->temp_img, segA_ptr->total_mask==0);
            imshow("Display Image", segA_ptr->temp_img);
        }
    }
    return;
}