#include <iostream>
#include <cv.hpp>

using namespace std;
using namespace cv;

int theObject[4];
Rect objectBondingRectangle = Rect{0,0,0,0};

//int to string helper function
string intToString(int number){

	//this function has a number input and string output
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void searchForMovement(Mat thresholdImage, Mat &cameraFeed){
    bool objectDetected = false;
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours(thresholdImage,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);

    if(contours.size()>0) objectDetected = true;
    else objectDetected = false;

    if(objectDetected){
        vector< vector<Point> > largestContourVal;
        largestContourVal.push_back(contours.at(contours.size()-1));

        objectBondingRectangle = boundingRect(largestContourVal.at(0));

        int objx = objectBondingRectangle.width;
        int objy = objectBondingRectangle.height;

        int xpos = objectBondingRectangle.x+objx/2;
        int ypos = objectBondingRectangle.y+objy/2;

        theObject[0] = xpos;
        theObject[1] = ypos;
        theObject[2] = objx;
        theObject[3] = objy;
    }

    int x = theObject[0];
    int y = theObject[1];
    int xlength = theObject[2];
    int ylength = theObject[3];
    Scalar color = Scalar(0,255,0);

    rectangle(cameraFeed,Point(x-xlength/2,y-ylength/2),Point(x+xlength/2,y+ylength/2),color);

    line(cameraFeed,Point(x,y),Point(x+25,y),color);
    line(cameraFeed,Point(x,y),Point(x-25,y),color);
    line(cameraFeed,Point(x,y),Point(x,y+25),color);
    line(cameraFeed,Point(x,y),Point(x,y-25),color);

    putText(cameraFeed,"Tracking object at (" + intToString(x)+","+intToString(y)+")",Point(x,y),1,1,Scalar(255,0,0),2);

}

int main()
{
    Mat grayImage1;
    Mat differenceImage;
    Mat thresholdImage;

    Mat white(500,500,CV_8UC1,Scalar(255,255,255));
    Mat img=imread("1.png");

    if(img.empty()){
        cout<<"Cannot read the image!"<<endl;
        return -1;
    }

    resize(img,img,Size(500,500));
    imshow("1.png",img);
    imshow("white",white);

    cvtColor(img,grayImage1,CV_BGR2GRAY);

    absdiff(grayImage1,white,differenceImage);

    threshold(differenceImage,thresholdImage,20,255,THRESH_BINARY);

    searchForMovement(thresholdImage,img);

    imshow("Frame",img);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
