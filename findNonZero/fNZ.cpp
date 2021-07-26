#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/highgui/highgui_c.h>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include<iomanip>

using namespace std;
using namespace cv;
#define PI 3.1415926


int frameWidth = 640;
int frameHeight = 480;


int main(int argc, char const *argv[]) {

    // Bring in image from file, must be in folder, and displaying the original
        Mat source = imread("InvertedLine.png");
        Mat Original = source.clone();
        cv::cvtColor(source,source,CV_BGR2HSV);
        cv::inRange(source, cv::Scalar(0,0,133), cv::Scalar(102,255,256), source);
        
    // mat container to receive images
        Mat destination;        //Destination is the Image we are operating on 
        int Centx;              //Variable for Storing X Value
        Mat Drawing;            //Image for Drawing Outut on 
        cv::Mat eElement=cv::getStructuringElement(cv::MORPH_RECT,cv::Size(2,2)); //Errosion Element
        cv::Mat dElement=cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3)); //Dialate


    //Sliders
        //Slider Box for REGION of INTREST:
        int Rectx=80,Recty=60,Rectw=280,Recth=75, ELx=source.cols/2;                                        
        cv::namedWindow("Sliders",cv::WINDOW_NORMAL);
        cv::resizeWindow("Sliders",300,300);
        cv::createTrackbar("ROI X:", "Sliders", &Rectx, source.cols-1);
        cv::createTrackbar("ROI Y:", "Sliders", &Recty, source.cols);
        cv::createTrackbar("ROI W:", "Sliders", &Rectw, source.cols);
        cv::createTrackbar("ROI H:", "Sliders", &Recth, source.rows);
        cv::createTrackbar("EgoLine:", "Sliders", &ELx,source.cols-1);
        

    while( true ) {
        //Creating a destination to represent the new frame 
            destination=source.clone();
        //Creating an image to draw on 
            Mat Drawing = Original.clone();
            cv::cvtColor(Drawing,Drawing, CV_BGR2GRAY);
            cv::cvtColor(Drawing,Drawing, CV_GRAY2RGB);
        //Defining the Region of Interest Rectangle;
            cv::Rect rect(Rectx,Recty,Rectw,Recth);
        //Erroding and Dialating the Image
            imshow("Before Erosion",destination);
            cv::erode(destination,destination,eElement);
            cv::dilate(destination,destination,dElement);
        //Determining the Contours
            std::vector<std::vector<cv::Point> > contours;
            cv::findContours( destination, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE );
            cv::Mat contourImage(destination.size(), CV_8UC1, cv::Scalar(0));
        //Determining Contour with Max Area
            double maxArea=0;
            int maxElement;
            if(contours.size()>0){
                int numObjects=contours.size();
                for(int i=0; i<contours.size(); i++){
                    double newArea = cv::contourArea(contours[i]);
                    if (newArea>maxArea){
                        maxArea = newArea;
                        maxElement=i;
                    }
                }
            }
            //Drawing the Largest Contour                            
            cv::drawContours(contourImage, contours, maxElement, cv::Scalar(255), CV_FILLED);
            imshow("Contour",contourImage);
         //Defining the Region of Interest
            cv::Mat ROI(contourImage, rect);
            imshow("ROI",ROI);
        //Determining the Non-Zero Pixel Values as a Vector of Coordinates
            std::vector<cv::Point2f> locations;   // output, locations of non-zero pixels 
            cv::findNonZero(ROI, locations);

        //If Statement that prevents Segfault if there are is no line detected in REGION of Interest
            if(locations.size()>0){
                //Determining the Mean X Value of All Coordinates
                    cv::Mat mean_;
                    cv::reduce(locations, mean_, 01, CV_REDUCE_AVG);
                    Centx=mean_.at<float>(0,0);
                //Outputting the Mean Pixel Value X Position in ROI frame
                    std::cout<<Centx<<std::endl;
                //Lateral Error Determination 
                    int LatE=Centx+Rectx-ELx;
                //Yaw Error Determination  -- To use atan values must be doubles
                    double LatEDoub = LatE, RowsDoub=destination.rows,y;
                    double OA = LatEDoub/(RowsDoub-(Recty-Recth/2));
                    double YawE=atan(OA)*180/PI;
                //Drawing Things 
                    //Region of Interest
                        cv::rectangle(Drawing, rect, cv::Scalar(  252, 153, 255),0.5,0 );
                    //Egoline
                        cv::line (Drawing, cv::Point(ELx,1), cv::Point(ELx,Drawing.rows-1),  cv::Scalar(158,230,117), 2, LINE_8, 0);
                    //Line Between Point and Egoline
                        cv::line (Drawing, cv::Point(ELx,Recty+Recth/2), cv::Point(Centx+Rectx,Recty+Recth/2), cv::Scalar(  128, 200, 255), 2, LINE_8, 0);
                    //Line Between Point and Base of Egoline
                        cv::line (Drawing, cv::Point(ELx,destination.rows-1),cv::Point(Centx+Rectx,Recty+Recth/2),cv::Scalar(  255, 160, 153), 2, LINE_8, 0);
                    //Circle of Point on Line (Detected Point)
                        cv::circle(Drawing,Point(Centx+Rectx,Recty+Recth/2),1,cv::Scalar(  89, 100, 255),2,4,0 );
                    //Circle at Base of Ego-Line
                        cv::circle(Drawing,Point(ELx,destination.rows-1),1,cv::Scalar(  89, 100, 255),2,4,0 );
                    //Printing Lateral Error on Image
                        std::string LatEstring=std::to_string(LatE);
                        putText(Drawing, LatEstring , Point(410, 350), FONT_HERSHEY_PLAIN, 1, cv::Scalar(  128, 200, 255),2,0);
                    //Printing Yaw Error on Image
                        std::string YawEstring=std::to_string(YawE);
                        putText(Drawing, YawEstring , Point(410, 335), FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 160, 153),2,0);
            }
            //Displaying Drawing
                imshow("Drawing",Drawing);
                waitKey(25);
    }


    return 0;
}   