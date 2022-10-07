#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <stack>
using namespace cv;
using namespace std;

class MouseCapture {
    vector<pair<int,int>> pvec;
    public:
        Mat cur_img;
        MouseCapture(Mat img): cur_img(img){}
        void draw_points(Mat img);
        static void mouse_callback (int, int, int, int, void*);
};

void MouseCapture::mouse_callback(int  event, int  x, int  y, int  flag, void *param)
{
    Mat temp_img;
    MouseCapture* mcap_ptr = (MouseCapture*)param;
    mcap_ptr->cur_img.copyTo(temp_img);
    if (event == EVENT_LBUTTONUP) {
        //cout << "(" << x << ", " << y << ")" << endl;
        pair<int,int> temp(x,y);
        mcap_ptr->pvec.push_back(temp);
        mcap_ptr->draw_points(temp_img);
        imshow("Display Image", temp_img);
    }
    else if (event == EVENT_RBUTTONUP){
        if (mcap_ptr->pvec.size()>0){
            mcap_ptr->pvec.pop_back();
            mcap_ptr->draw_points(temp_img);
            imshow("Display Image", temp_img);
        }
    }
}

void MouseCapture::draw_points(Mat img){
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
    //cout << image_paths[0] << image_paths[1] << endl;
    int i = 0;
    while(true){
        Mat image;
        //image = imread( argv[1], 1 );
        image = imread( image_paths[i], 1 );
        if ( !image.data )
        {
            printf("No image data \n");
            return -1;
        }
        MouseCapture mcap(image);
        namedWindow("Display Image", WINDOW_AUTOSIZE );
        setMouseCallback("Display Image", mcap.mouse_callback, &mcap);

        imshow("Display Image", image);
        int key = waitKey(0);
        if (char(key) == 'q'){
            break;
        }
        else if (char(key) == 'a'){
            i = (i-1)%image_paths.size();
        }
        else if (char(key) == 'd'){
            i = (i+1)%image_paths.size();
        }
    }
    return 0;
}