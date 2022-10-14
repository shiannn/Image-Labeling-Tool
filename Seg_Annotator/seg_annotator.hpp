#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class SegAnnotator{
    public:
        Mat total_mask;
        Mat temp_mask;
        Mat temp_img;
        vector<vector<Mat> > deltas;
        vector<Mat> cur_imgs;
        vector<Mat> pre_imgs;
        vector<String> img_paths;
        int cur_idx;
        bool drawing;
        SegAnnotator(vector<String> img_paths);
        static void seg_callback(int  event, int  x, int  y, int  flag, void *param);
        void draw_mask();
};