## A Simple Image Labeling Tool
1. Installation
    - Install Opencv
    - Add the **Absolute path** to OpenCV folder containing **OpenCVConfig.cmake** to **config_vars.txt**
    - Cmake
        ```bash
        # In the project directory
        mkdir build/
        cd build/
        cmake ../
        ```
    - Build
        ```bash
        # In build/
        make
        ```
2. Run
    - Select the directory containing images to be labeled
        ```bash
        # In build/
        ./DisplayImage "<Directory Path containing images>/*.jpg"
        ```
    - Select **Bounding Boxes** or **Segmentation Masks** labeling following the program

    - Select the path for saving segmentation masks. The directory will be created if not existed.

### Bounding Boxes Labeling
- Press **"a"** & **"d"** for the **previous** and **next** image
- **Left button on Mouse** for adding a point of bounding-box
- **Right button on Mouse** for undoing

    <img src=demo/bbox_fast.gif>

    You will get a **label.csv** containing the coordinates. The $i$'th bounding box is composed of two points $(x_i^1, y_i^1)$, $(x_i^2, y_i^2)$.
    
    image name | $x_1^1$ | $y_1^1$ | $x_1^2$ | $y_1^2$ | $x_2^1$ | $y_2^1$ | ...
    ---|---|---|---|---|---|---|---|
  arrow.jpg | 293 | 189 | 485 | 479 | 496 | 328 | 628 | 517 | 648 | 375 | 774 | 549 | 786 | 387 | 861 | 587 | 869 | 404 | 908 | 453 | 870 | 458 | 907 | 593 | 929 | 478 | 1025 | 626 | 1038 | 522 | 1128 | 694
  text.png | 40 | 71 | 82 | 122 | 91 | 70 | 130 | 124 | 135 | 71 | 174 | 123 | 180 | 70 | 221 | 121 | 250 | 71 | 290 | 122 | 297 | 71 | 342 | 122 | 348 | 68 | 399 | 123 | 405 | 67 | 450 | 124
  triangular_sign.png | 224 | 485 | 322 | 587 | 327 | 481 | 415 | 592 | 421 | 480 | 507 | 586 | 510 | 480 | 613 | 594


- Press **"q"** for quiting the program

### Semgentation Masks Labeling
- Press **"a"** & **"d"** for the **previous** and **next** image
- Press **"s"** & **"w"** for the **previous** and **next** label index
- Press **"1"** & **"2"** for the **thinner** and **thicker** brush
- **Left button on Mouse** for labeling a segmentation mask. Drag the mouse for larger area.
- **Right button on Mouse** for undoing

    <img src=demo/switch_images.gif>

    You will get segmentation mask results like the following.
    <img src=demo/arrow_mask.png width=300>
    <img src=demo/text_mask.png width=150>

- Press **"q"** for quiting the program