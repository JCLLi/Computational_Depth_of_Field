#include <opencv2/opencv.hpp>

int fontace = cv::FONT_HERSHEY_SIMPLEX;
double fontScale = 0.5;
Scalar textColor(0, 0, 0);  // Black color
cv::Scalar colors[5] = {Scalar(255, 0, 0), Scalar(255, 0, 100), Scalar(0, 0, 255), Scalar(0, 255, 0), Scalar(255, 255, 255)};

// Fixed shown text in UI
void default_txt(Mat ui){
    std::string fu_text = "Function: ";    
    Point fu_textPosition(50, 50);
    putText(ui, fu_text, fu_textPosition, fontace, fontScale, textColor, 2);

    std::string s_text = "Scribble strength: ";    
    Point s_textPosition(50, 100);
    putText(ui, s_text, s_textPosition, fontace, fontScale, textColor, 2);

    std::string f_text = "Focus point: ";    
    Point f_textPosition(250, 100);
    putText(ui, f_text, f_textPosition, fontace, fontScale, textColor, 2);

    std::string a_text = "Apeture: F";    
    Point a_textPosition(50, 150);
    putText(ui, a_text, a_textPosition, fontace, fontScale, textColor, 2);

    std::string i_text = "Sribble strength has 5 levels:  ";
    std::string i_textb = "0(the least):blue,";
    std::string i_textp = "1:purple,";
    std::string i_textr = "2:red,";
    std::string i_textg = "3:green,";
    std::string i_textw = "4(the strongest):white";
    std::string i_text1 = "A higher level means deeper in the image.";    
    std::string i_text2 = "Focus point can be any points on the image. ";
    std::string i_text3 = "You can't run processing step before choosing a focus point.";
    std::string i_text4 = "Apeture can be varied from 17 - 1. ";
    std::string i_text5 = "The bigger apeture is, the more areas other than the focus will be blurred.";
    std::string i_text6 = "Press 's' to scribble on the image. Press 'q' to change scribble strength. ";
    std::string i_text7 = "Press 'f' to choose focus point. Press 'a' to change apeture size.";
    std::string i_text8 = "Press 'e' to choose pixels need to be ignored. Press 'esc' to end editing.";
    Point i_textPosition(30, 200);
    Point i_textPositionb(30, 220);
    Point i_textPositionp(180, 220);
    Point i_textPositionr(270, 220);
    Point i_textPositiong(330, 220);
    Point i_textPositionw(420, 220);
    Point i_textPosition1(30, 240);
    Point i_textPosition2(30, 280);
    Point i_textPosition3(30, 300);
    Point i_textPosition4(30, 340);
    Point i_textPosition5(30, 360);
    Point i_textPosition6(30, 400);
    Point i_textPosition7(30, 420);
    Point i_textPosition8(30, 440);
    putText(ui, i_text, i_textPosition, fontace, 0.5, textColor, 2);
    putText(ui, i_textb, i_textPositionb, fontace, 0.5, colors[0], 2);
    putText(ui, i_textp, i_textPositionp, fontace, 0.5, colors[1], 2);
    putText(ui, i_textr, i_textPositionr, fontace, 0.5, colors[2], 2);
    putText(ui, i_textg, i_textPositiong, fontace, 0.5, colors[3], 2);
    putText(ui, i_textw, i_textPositionw, fontace, 0.5, textColor, 2);
    putText(ui, i_text1, i_textPosition1, fontace, 0.5, textColor, 2);
    putText(ui, i_text2, i_textPosition2, fontace, 0.5, textColor, 2);
    putText(ui, i_text3, i_textPosition3, fontace, 0.5, colors[2], 2);
    putText(ui, i_text4, i_textPosition4, fontace, 0.5, textColor, 2);
    putText(ui, i_text5, i_textPosition5, fontace, 0.5, textColor, 2);
    putText(ui, i_text6, i_textPosition6, fontace, 0.5, textColor, 2);
    putText(ui, i_text7, i_textPosition7, fontace, 0.5, textColor, 2);
    putText(ui, i_text8, i_textPosition8, fontace, 0.5, textColor, 2);
}

// Run UI and update UI text
void run_UI(int color_code, Point focus, int apeture, int function, std::string update){
    Mat updated_UI(500, 800, CV_8UC3, cv::Scalar(255, 255, 255));

    Point u_textPosition(200, 20);
    Scalar u_textColor(0, 0, 255);  // Black color
    putText(updated_UI, update, u_textPosition, fontace, fontScale, u_textColor, 2);

    std::string fu_text = "scribble";
    if(function == 1)
        fu_text = "choose focus";
    else if(function == 3)
        fu_text = "choose ignore points";
    Point fu_textPosition(150, 50);
    putText(updated_UI, fu_text, fu_textPosition, fontace, fontScale, textColor, 2);

    std::string s_text = std::to_string(color_code);
    Point s_textPosition(200, 100);
    if(color_code != 4)
        putText(updated_UI, s_text, s_textPosition, fontace, fontScale, colors[color_code], 2);
    else
        putText(updated_UI, s_text, s_textPosition, fontace, fontScale, textColor, 2);
    
    std::string f_text = std::to_string(focus.x) + ", " + std::to_string(focus.y);
    if(focus.x == -1) f_text = "no focus ";
    Point f_textPosition(360, 100);
    putText(updated_UI, f_text, f_textPosition, fontace, fontScale, textColor, 2);

    std::string a_text = std::to_string(18 - apeture);    
    Point a_textPosition(150, 150);
    putText(updated_UI, a_text, a_textPosition, fontace, fontScale, textColor, 2);

    default_txt(updated_UI);
    imshow("UI", updated_UI);
}

void start_ui(){
    cv::Mat ui(500, 800, CV_8UC3, cv::Scalar(255, 255, 255));
    std::string fu_text = "scribble";    
    Point fu_textPosition(150, 50);
    putText(ui, fu_text, fu_textPosition, fontace, fontScale, textColor, 2);

    std::string s_text = std::to_string(0);
    Point s_textPosition(200, 100);
    putText(ui, s_text, s_textPosition, fontace, fontScale, colors[0], 2);

    std::string f_text = "no focus ";    
    Point f_textPosition(360, 100);
    putText(ui, f_text, f_textPosition, fontace, fontScale, textColor, 2);

    std::string a_text = "17";    
    Point a_textPosition(150, 150);
    putText(ui, a_text, a_textPosition, fontace, fontScale, textColor, 2);

    default_txt(ui);
    imshow("UI", ui);
}

