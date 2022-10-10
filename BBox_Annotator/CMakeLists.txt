add_library(bbox bbox_annotator.cpp)
target_link_libraries( bbox ${OpenCV_LIBS} )