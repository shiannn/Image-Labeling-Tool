#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class SegAnnotator{
    public:
        Mat total_mask;
        Mat temp_mask;
        Mat temp_img;
        Mat cur_img;
        vector<vector<SparseMat> > deltas;
        vector<String> img_paths;
        int cur_idx;
        int color_idx;
        int brush_size;
        bool drawing;
        SegAnnotator(vector<String> img_paths);
        static void seg_callback(int  event, int  x, int  y, int  flag, void *param);
        void draw_mask();
};