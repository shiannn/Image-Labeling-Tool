#include <stdio.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <stack>
#include <fstream>
#include <filesystem>
#include "BBox_Annotator/bbox_annotator.hpp"
#include "Seg_Annotator/seg_annotator.hpp"

using namespace cv;
using namespace std;
namespace fs = filesystem;

int add_masks(string image_folder_path, string output_mask_path){
    fs::path mask_output_dir = fs::path(output_mask_path);
    fs::create_directory(mask_output_dir);
    vector<String> image_paths;
    glob(image_folder_path, image_paths, false);
    SegAnnotator seg_annotator(image_paths);
    while(true){
        int i = seg_annotator.cur_idx;
        Mat image;
        //image = imread( argv[1], 1 );
        image = imread( image_paths[i], 1 );
        if ( !image.data )
        {
            printf("No image data \n");
            return -1;
        }
        fs::path output_name = fs::path(image_paths[i]).stem();
        output_name += "_mask.png";
        fs::path output_path = mask_output_dir / output_name;
        image.copyTo(seg_annotator.temp_img);
        image.copyTo(seg_annotator.cur_img);
        if(fs::exists(output_path)){
            seg_annotator.total_mask = imread(output_path, IMREAD_GRAYSCALE);
            Mat img_color;
            applyColorMap(seg_annotator.total_mask, img_color, COLORMAP_JET);
            img_color.copyTo(seg_annotator.temp_img, seg_annotator.total_mask>0);
            //seg_annotator.temp_img.setTo(Scalar(255,0,0), seg_annotator.total_mask>0);
            //image.setTo(Scalar(255,0,0), seg_annotator.total_mask>0);
        }else{
            seg_annotator.total_mask = Mat(image.size(), CV_8UC1, Scalar(0));
        }
        seg_annotator.temp_mask = Mat(image.size(), CV_8UC1, Scalar(0));
        // TODO: need to draw the mask results here
        namedWindow("Display Image", WINDOW_AUTOSIZE );
        setMouseCallback("Display Image", seg_annotator.seg_callback, &seg_annotator);

        imshow("Display Image", seg_annotator.temp_img);
        int key = waitKey(0);
        if (char(key) == 'q'){
            break;
        }
        else if (char(key) == 'a'){
            seg_annotator.cur_idx = (seg_annotator.cur_idx-1+image_paths.size())%image_paths.size();
        }
        else if (char(key) == 'd'){
            seg_annotator.cur_idx = (seg_annotator.cur_idx+1)%image_paths.size();
        }
        else if (char(key) == 'w'){
            seg_annotator.color_idx = (seg_annotator.color_idx+17)%255;
            cout << seg_annotator.color_idx << endl;
        }
        else if (char(key) == 's'){
            seg_annotator.color_idx = (seg_annotator.color_idx-17+255)%255;
            cout << seg_annotator.color_idx << endl;
        }
        else if (char(key) == '1'){
            if (seg_annotator.brush_size > 1){
                seg_annotator.brush_size -= 1;
            }
            cout << seg_annotator.brush_size << endl;
        }
        else if (char(key) == '2'){
            if (seg_annotator.brush_size < 15){
                seg_annotator.brush_size += 1;
            }
            cout << seg_annotator.brush_size << endl;
        }
        
        imwrite(output_path, seg_annotator.total_mask);
    }
    return 0;
}
int add_bboxes(string image_folder_path){   
    vector<String> image_paths;
    glob(image_folder_path, image_paths, false);
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

int main(int argc, char** argv ){
    if (argc != 2){
        cout << "usage: ./DisplayImage <Path of Image Folder>" << endl;
        return -1;
    }
    int usage;
    cout << "1: Add Bounding Boxes 2: Add Segmentation Masks" << endl;
    cin >> usage;
    if (usage != 1 && usage != 2){
        cout << "Please enter 1 or 2" << endl;
        return -1;
    }
    string image_folder_path(argv[1]);
    string output_mask_path;
    if (!fs::exists(image_folder_path)){
        cout << "Image Folder not exists" << endl;
        return -1;
    }
    else if (usage == 1){
        add_bboxes(image_folder_path);
    } else if (usage == 2){
        cout << "Please Enter the Path of Masks Output Folder:" << endl;
        cin >> output_mask_path;
        if (!fs::exists(image_folder_path)){
            cout << "Masks Output Folder not exists" << endl;
            fs::create_directories(image_folder_path);
        }
        add_masks(image_folder_path, output_mask_path);
    }
}