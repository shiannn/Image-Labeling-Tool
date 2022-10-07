### A Simple Image Labeling Tool
1. Installation
    - Install Opencv
    - Cmake
        ```bash
        # In the project directory
        mkdir build/
        cd build/
        cmake -DOpenCV_DIR=<OpenCV Folder Containing OpenCVConfig.cmake> ../
        ```
    - Build
        ```bash
        # In build/
        make
        ```
2. Run
    ```bash
    # In build/
    ./DisplayImage "<Directory Path containing images>/*.jpg"
    ```
3. usage
    - Press **"a"** & **"d"** for the **previous** and **next** image
    - **Left button on Mouse** for adding a point of bounding-box
    - **Right button on Mouse** for removing a point of bounding-box