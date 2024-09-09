#include "Diffusion.h"

// To get a value with a giving x using normal distribution, same function from lab3
float gauss(const float x, const float sigma = 1.0f, const float mu = 0.0f){   
    auto exponent = (x - mu) / sigma;
    return std::exp(-0.5f * exponent * exponent);
}

/**
 * Blur the image according to depth map and user defined focous and aperture. Different depths are blurred differently.
 * w_i = gauss(dist, sigma_dist) * gauss(diff_value, sigma_value) is the key of cross_bilateral_filter. Sigma_dist and sigma_value are decided
 * by apeture size and depth difference between aimed pixel and neighbour pixel.
 * 
 * The big difference bewteen this one and normal bilateral filter is the calculation diff_value. In our case, it is calulated according to the 
 * depth map but normal bilateral filter calculte diff_value according to the pixel values of the original image.
 * 
 * 
 * param depth: a depth map
 * param image: the original input image
 * param focus: focus point chosen by user
 * param apeture: apeture size chosen by user
 * return: a blurred image
*/
Mat cross_bilateral_filter(Mat depth, Mat image, Point focus, int apeture){
    std::cout << "Blurring.. \n";
    float focus_depth = depth.at<float>(focus.y, focus.x); // Get the depth of the focus point

    const int size = 15;
    const float sigma_dist = std::sqrtf(apeture); // The bigger apeture is, the bigger sigma is, the more image will be blurred.
    
    int height = image.rows;
    int width = image.cols;
    Mat output(height, width, CV_32FC3, cv::Scalar(0, 0, 0));

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(abs(depth.at<float>(y, x) - focus_depth) > 10){ // Chose a depth range based on focus point, this range will not be blurred, make output looks more natural.

                float sigma_value = abs(std::pow(depth.at<float>(y, x) - focus_depth, 2)); // Sigma_value depends on difference of depth between current pixel and focus point

                int interval = (size - 1) / 2;
                int xs = x - interval;
                int xe = x + interval;
                int ys = y - interval;
                int ye = y + interval;
                if (xe > width - 1)
                    xe = width - 1;
                if (xs < 0)
                    xs = 0;
                if (ye > height - 1)
                    ye = height - 1;
                if (ys < 0)
                    ys = 0;
                float sum[] = {0, 0, 0};
                float num = 0;
                for (int index_y = ys; index_y <= ye; index_y++) {
                    for (int index_x = xs; index_x <= xe; index_x++) {
                        // Start to calculate two gaussian functions
                        float dist = sqrtf((float)(pow((float)(index_y - y), 2) + pow((float) (index_x - x), 2)));
                        float diff_value = 0;

                        float diff = depth.at<float>(y, x) - depth.at<float>(index_y, index_x);
                        diff_value += diff * diff;
                        
                        diff_value = sqrtf(diff_value);
                        float ga = gauss(dist, sigma_dist);
                        float gb = gauss(diff_value, sigma_value);
                        sum[0] += ga * gb * image.at<Vec3f>(index_y, index_x)[0]; // weight * image values
                        sum[1] += ga * gb * image.at<Vec3f>(index_y, index_x)[1];
                        sum[2] += ga * gb * image.at<Vec3f>(index_y, index_x)[2];
                        num += ga * gb;
                    }
                }
                    
                output.at<Vec3f>(y, x)[0] = sum[0] / num; // Normalize
                output.at<Vec3f>(y, x)[1] = sum[1] / num;
                output.at<Vec3f>(y, x)[2] = sum[2] / num;
            }
            else{
                output.at<Vec3f>(y, x)[0] = image.at<Vec3f>(y, x)[0];
                output.at<Vec3f>(y, x)[1] = image.at<Vec3f>(y, x)[1];
                output.at<Vec3f>(y, x)[2] = image.at<Vec3f>(y, x)[2];
            }
        }
    }
    return output;
}

