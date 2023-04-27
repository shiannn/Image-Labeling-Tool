#include <fstream>
#include <opencv2/opencv.hpp>
#include "bbox_annotator.hpp"
using namespace cv;
using namespace std;

MouseCapture::MouseCapture(vector<String> img_paths):img_paths(img_paths){
    int n_imgs = img_paths.size();
    cur_imgs.resize(n_imgs);
    pvecs.resize(n_imgs);
    cur_idx = 0;
}

void MouseCapture::mouse_callback(int  event, int  x, int  y, int  flag, void *param)
{
    Mat temp_img;
    MouseCapture* mcap_ptr = (MouseCapture*)param;
    int cur_idx = mcap_ptr->cur_idx;
    mcap_ptr->cur_imgs[cur_idx].copyTo(temp_img);
    if (event == EVENT_LBUTTONUP) {
        pair<int,int> temp(x,y);
        mcap_ptr->pvecs[cur_idx].push_back(temp);
        mcap_ptr->draw_points(temp_img);
        imshow("Display Image", temp_img);
    }
    else if (event == EVENT_RBUTTONUP){
        if (mcap_ptr->pvecs[cur_idx].size()>0){
            mcap_ptr->pvecs[cur_idx].pop_back();
            mcap_ptr->draw_points(temp_img);
            imshow("Display Image", temp_img);
        }
    }
}

void MouseCapture::draw_points(Mat img){
    vector<pair<int,int> > pvec = pvecs[cur_idx];
    for (int i=0; i<pvec.size() ; i++){
        int x = pvec[i].first;
        int y = pvec[i].second;
        Point center = Point(x,y);
        circle(img, center, 1, CV_RGB(255,0,0),3);
        if (i % 2 == 1) {
            // draw bbox
            Point prev = Point(pvec[i-1].first,pvec[i-1].second);
            rectangle(img, prev, center, CV_RGB(255,0,0));
        }
    }
}

void MouseCapture::write2csv(){
    ofstream output_csv;
    output_csv.open("label.csv");
    for (int i=0;i<img_paths.size();i++){
        output_csv << img_paths[i];
        int total_pts = pvecs[i].size();
        if(total_pts % 2 != 0) total_pts -= 1;
        for (int j=0;j<total_pts;j++){
            pair<int,int> pt = pvecs[i][j];
            int x = pt.first;
            int y = pt.second;
            output_csv << "," << x << "," << y;
        }
        output_csv << endl;
    }
    return;
}
