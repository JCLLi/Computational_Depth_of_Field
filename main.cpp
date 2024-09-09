#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

#include "Cross_Bilateral_Filter.h"

using namespace cv;

std::vector<Point> drawnPoints; 

Mat image;

Point start, end;
Point focus = Point(-1, -1);

cv::Scalar color[5] = {Scalar(255, 0, 0), Scalar(255, 0, 100), Scalar(0, 0, 255), Scalar(0, 255, 0), Scalar(255, 255, 255)};
cv::Scalar e_color = Scalar(0, 255, 255);
cv::Scalar line_color = color[0];

bool drawing = false;
bool chosen = false;

int index = 0;
int function_state = 0;
int aperture = 1;

struct MatList{
    Mat scribble;
    Mat ignore;
};

// Mouse movement capture function
void onMouse(int event, int x, int y, int flags, void* param) {
    MatList mat_list = *static_cast<MatList*>(param);

    // When user choose scribble or choose ignored point functions (when mouse left button is pressed)
    if (event == EVENT_FLAG_LBUTTON && (function_state == 0 || function_state == 3)) {
        start = Point(x, y);
        drawing = true;
        // Add the starting point to the vector when you begin drawing
        drawnPoints.push_back(start);
    }
    // When user choose scribble or choose ignored point functions (when mouse is moving)
    else if (event == EVENT_MOUSEMOVE && drawing && (function_state == 0 || function_state == 3)) {
        end = Point(x, y);
        drawnPoints.push_back(end);
        if(function_state == 0)
            line(mat_list.scribble, start, end, line_color, 3); // Draw a line according to different depth strength
        else if(function_state == 3)
            line(mat_list.ignore, start, end, e_color, 3);// Draw a line according to show ignored pixels
        start = end;
        imshow("Image", image + mat_list.scribble + mat_list.ignore);
    }
    // When user choose scribble or choose ignored point functions (when mouse left button is released)
    else if (event == EVENT_LBUTTONUP && (function_state == 0 || function_state == 3)) {
        drawing = false;
    }
    else if(event == EVENT_FLAG_LBUTTON && function_state == 1){ // Choose a focus point
        chosen = true;
        focus = Point(x, y);
        run_UI(index, focus, aperture, function_state, "");
    }
}

int main(int, char**){
    std::string imagePath;
    std::cout << "Enter the file path of the image you want to load: ";
    std::cin >> imagePath;

    // Read a image according to a given path
    image = imread(imagePath);

    if (!image.data) {
        std::cerr << "Error: Unable to load the image." << std::endl;
        return -1;
    }


    image.convertTo(image, CV_32FC3);
    cv::normalize(image, image, 0.0, 1.0, cv::NORM_MINMAX);
    Mat scribble = Mat::zeros(image.size(), CV_32FC3);  // Create a transparent mat for scribbles
    Mat ignore = Mat::zeros(image.size(), CV_32FC3); // Create a transparent mat for chosing ignored pixels
    imshow("Image", image);
    
    MatList mat_list = {scribble, ignore};

    start_ui(); //Start UI
    setMouseCallback("Image", onMouse, &mat_list); // Start capturing mouse movements

    while(1){ //Waiting for user input from keyboard
        int key = waitKey(10);
        if(key == 115){
            function_state = 0; // Key 's' is pressed, function scribble
            run_UI(index, focus, aperture, function_state, "");
        }
            
        if(key == 102){
            function_state = 1; // Key 'f' is pressed, function choose focus
            run_UI(index, focus, aperture, function_state, "");
            
        }
            
        if(key == 113){ // Key 'q' is pressed, change depth strength (5 level in total. level-0(the least):blue , level-1:purple, 
                        // level-2:red, level-3:green, level-4(the strongest):white)
            if(index == 4)
                index = 0;
            else
                index++; 
            line_color = color[index];
            run_UI(index, focus, aperture, function_state, "");
        }

        if(key == 97){ // Key 'a' is pressed, change aperture size value is from 1-17;
            aperture += 2;
            if(aperture > 16)
                aperture = 1;
            run_UI(index, focus, aperture, function_state, "");
        }

        if(key == 101){ // Key 'e' is pressed, function choose ignored points
            function_state = 3;
            run_UI(index, focus, aperture, function_state, "");
        }

        if(key == 27){ // Key 'ESC' is pressed, start image processing
            if(chosen){
                break;
            }else{// User must choose a focus point
                std::string update = "Please choose focus first!!!!";
                run_UI(index, focus, aperture, function_state, update);
            }
            
        }
    }
    
    // Create a binary mask from the drawn shape, and a mask for ignored pixed
    cvtColor(scribble, scribble, COLOR_BGR2GRAY);
    imwrite("../../output/1a-scribble.png", scribble);   
    imwrite("../../output/1b-ignore.png", ignore);  

    // Using anisotropic diffusion to get depth map
    Mat depth = anisotropic(scribble, image, ignore);
    imwrite("../../output/4-depth.png", depth);

    // Blur the image according to depth map and user defined focous and aperture
    Mat result = cross_bilateral_filter(depth, image, focus, aperture);

    Mat image8u;
    result.convertTo(image8u, CV_8U, 255.0); 
    imwrite("../../output/5-output.png", image8u);
    
    return 0;
}
