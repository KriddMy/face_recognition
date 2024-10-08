﻿#include <iostream>
#include <dlib/opencv.h>
#include <dlib/dnn.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "face_recognizer.h"
#include "main_window.h"

using namespace dlib;
using namespace std;

const string faces_folder = "faces_to_train";

int main() try
{
    init(faces_folder);

    cv::VideoCapture cap(0);
    const unsigned int maxDevicesTries = 5;
    for(int i = 1; i < maxDevicesTries && !cap.isOpened(); i++) {
        cerr << "Unable to connect to default camera. Trying camera #" << i << "..." << endl;
        cap.open(i);
    }

    if (!cap.isOpened()) {
        cerr << "Unable to connect to camera. Press any key to exit." << endl;
        return 1;
    }
    
    main_window win;
    resnet_face_recognizer recognizer;
    while (!win.is_closed()) {
        cv::Mat frame;
        cv::Mat forProcessing;

        if (!cap.read(frame))
            break;

        double scale = 600.0f / frame.rows;
        cv::resize(frame, forProcessing, cv::Size(), scale, scale);

        cv_image<bgr_pixel> cimg(forProcessing);
        matrix<rgb_pixel> img;
        assign_image(img, cimg);

        win.set_image(img);

        std::vector<matrix<rgb_pixel>> faces;
        std::vector<full_object_detection>shapes;
        shapes = recognizer.get_faces_chips(faces, img);

        if (faces.size() == 0)
        {
            cout << "No faces found in image!" << endl;
            win.clear_overlay();
            continue;
        }

        if (win.IsAdding)
        {
            win.clear_overlay();
            win.add_overlay(shapes);
            cout << "saving photo \n";
            recognizer.save_face_to_folder(faces_folder, win.GetPersonName(), faces);
        }
        else
        {
            std::vector<std::string> names;
            recognizer.detect_faces(faces_folder, faces, names);

            win.clear_overlay();
            win.add_overlay(shapes, names);
        }
    }
}
catch (std::exception& e)
{
    cout << e.what() << endl;
}