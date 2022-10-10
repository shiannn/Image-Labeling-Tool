#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class SegAnnotator{
    public:
        vector<Mat> cur_imgs;
        vector<String> img_paths;
        int cur_idx;
        bool drawing;
        SegAnnotator(vector<String> img_paths);
        static void seg_callback(int  event, int  x, int  y, int  flag, void *param);
};