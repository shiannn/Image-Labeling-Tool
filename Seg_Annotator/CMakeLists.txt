add_library(seg seg_annotator.cpp)
target_link_libraries( seg ${OpenCV_LIBS} )