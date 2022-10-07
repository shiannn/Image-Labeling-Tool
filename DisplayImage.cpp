#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <stack>
#include <fstream>
using namespace cv;
using namespace std;

class MouseCapture {
    public:
        vector<vector<pair<int,int>>> pvecs;
        vector<Mat> cur_imgs;
        vector<String> img_paths;
        int cur_idx;
        MouseCapture(vector<String> img_paths):img_paths(img_paths){
            int n_imgs = img_paths.size();
            cur_imgs.resize(n_imgs);
            pvecs.resize(n_imgs);
            cur_idx = 0;
        }
        void draw_points(Mat img);
        void write2csv();
        static void mouse_callback (int, int, int, int, void*);
};

void MouseCapture::mouse_callback(int  event, int  x, int  y, int  flag, void *param)
{
    Mat temp_img;
    MouseCapture* mcap_ptr = (MouseCapture*)param;
    int cur_idx = mcap_ptr->cur_idx;
    mcap_ptr->cur_imgs[cur_idx].copyTo(temp_img);
    if (event == EVENT_LBUTTONUP) {
        //cout << "(" << x << ", " << y << ")" << endl;
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
    vector<pair<int,int>> pvec = pvecs[cur_idx];
    for (int i=0; i<pvec.size() ; i++){
        //cout << mcap_ptr->pvec.back().first << " " << mcap_ptr->pvec.back().second << endl;
        int x = pvec[i].first;
        int y = pvec[i].second;
        Point center = Point(x,y);
        //circle(mcap_ptr->cur_img, center, 1, CV_RGB(255,0,0),3);
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
    output_csv << "header";
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