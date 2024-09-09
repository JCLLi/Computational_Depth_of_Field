#include <opencv2/opencv.hpp>
#include <omp.h>
#include <vector>
#include <chrono>
using namespace cv;

#include "UI.h"

/**
 * Using anisotropic diffusion to get a depth map. 
 * 
 * param scribble: a mat with all user scribbles
 * param image: the original input image
 * param ignore: a mat with info of depth calculation ignoring pixels
 * 
 * return: a depth map is return
*/
Mat anisotropic(Mat scribble, Mat image, Mat ignore){

    int width = scribble.cols;
    int height = scribble.rows;
    
    std::vector<Point> mask_points; 
  
    // Collect scribble points to a vector
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            if(scribble.at<float>(y, x) != 0)
                mask_points.push_back(Point(x, y)); 
        }
    }
    std::cout << "Creating mask\n";
    // Keep scribble points and make other places into a light color
    Mat temp = scribble;
    #pragma omp parallel for
    for (int y = 0; y < height; y++){
        #pragma omp parallel for
        for (int x = 0; x < width; x++){
            std::vector<Point>::iterator it = std::find(mask_points.begin(), mask_points.end(), Point(x, y));
            if(it == mask_points.end())
                temp.at<float>(y, x) = 250;
        }
    }
    Mat mask = temp;
    imwrite("../../output/2-mask.png", mask);
    
    Mat brightness(height, width, CV_32FC1, cv::Scalar(0));
    std::cout << "Getting brightness\n";
    // Get the brightness of the picture. This is according to the code of lab1 which is used to get luminance 
    #pragma omp parallel for
    for (int y = 0; y < height; y++){
        #pragma omp parallel for
        for (int x = 0; x < width; x++){
            brightness.at<float>(y, x) = image.at<Vec3f>(y, x)[0] * 0.114 + image.at<Vec3f>(y, x)[1] * 0.587 + image.at<Vec3f>(y, x)[2] * 0.299;
        }
    }

    Mat image8u;
    brightness.convertTo(image8u, CV_8U, 255.0); 
    imwrite("../../output/3-brightness.png", image8u);
    Mat output(height, width, CV_32FC1, cv::Scalar(0));

    // Anisotropic Diffusion
     std::cout << "Getting depth map\n";
    float beta = -60; // The parameter used to limit the diffusion. The bigger abs(beta) is, the less scribble will be diffused.
    int it = 5000; // Iteration times
    for(int i = 0; i < it; i++){
        if(i % 200 == 0){
            String update = "Progress: " +  std::to_string(100 * i / it) + "% done.\n";
            std::cout << update;
        }
            
        #pragma omp parallel for
        for (int y = 0; y < height; y++){
            #pragma omp parallel for
            for (int x = 0; x < width; x++){
                if(mask.at<float>(y, x) == 250){// Check all points which are not scribbles. (Scribbles are taken as hard limit)
                    if(ignore.at<Vec3f>(y, x)[0] == 0 && ignore.at<Vec3f>(y, x)[2] == 255 && i > 2000){// Check if the pixel is a ignored point(Fade edge and gradient details).
                        float avg = 0;
                        float total = 0;
                        if(x - 1 >= 0){
                            avg += temp.at<float>(y, x - 1);
                            total++;
                        }
                        if(x + 1 < width - 1){
                            avg += temp.at<float>(y, x + 1);   
                            total++;
                        }
                        if(y + 1 < height - 1){
                            avg += temp.at<float>(y + 1, x); 
                            total++;
                        }
                        if(y - 1 >= 0){
                            avg += temp.at<float>(y - 1, x);  
                            total++;
                        }
                        output.at<float>(y, x) = avg / total; // Using poisson for ignored pixels
                    }else{
                        float omega_sum = 0;
                        float weight_sum = 0;
                        if(x - 1 >= 0){
                            float omega = exp(beta * abs(brightness.at<float>(y, x) - brightness.at<float>(y, x - 1)));
                            weight_sum += omega * temp.at<float>(y, x - 1);
                            omega_sum += omega;
                        }
                        if(x + 1 < width - 1){
                            float omega = exp(beta * abs(brightness.at<float>(y, x) - brightness.at<float>(y, x + 1)));
                            weight_sum += omega * temp.at<float>(y, x + 1);   
                            omega_sum += omega;
                        }
                        if(y + 1 < height - 1){
                            float omega = exp(beta * abs(brightness.at<float>(y, x) - brightness.at<float>(y + 1, x)));
                            weight_sum += omega * temp.at<float>(y + 1, x); 
                            omega_sum += omega;
                        }
                        if(y - 1 >= 0){
                            float omega = exp(beta * abs(brightness.at<float>(y, x) - brightness.at<float>(y - 1, x)));
                            weight_sum += omega * temp.at<float>(y - 1, x);  
                            omega_sum += omega;
                        }
                        output.at<float>(y, x) = weight_sum / omega_sum; // Anisotropic diffusion  D = (w1D1 + w2D2 + w3D3 + w4D4) / (w1 + w2 + w3 + w4), 
                                                                         // w is omega, D is the center pixel, 1 2 3 4 are four neighbours.
                    }
                }else{
                    output.at<float>(y, x) = temp.at<float>(y, x);// Scribble pixel is kept same
                }
            }
        }
        temp = output;
    }
    
    std::string update = "Progress: " +  std::to_string(100 * it / it) + "% done.\n";
    std::cout << update;

    return output;
}