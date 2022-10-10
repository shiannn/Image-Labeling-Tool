#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class MouseCapture {
    public:
        vector<vector<pair<int,int> > > pvecs;
        vector<Mat> cur_imgs;
        vector<String> img_paths;
        int cur_idx;
        MouseCapture(vector<String> img_paths);
        void draw_points(Mat img);
        void write2csv();
        static void mouse_callback (int, int, int, int, void*);
};