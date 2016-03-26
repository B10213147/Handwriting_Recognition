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

    cvtColor(img,grayImage1,CV_BGR2GRAY);

    absdiff(white,grayImage1,differenceImage);

    threshold(differenceImage,thresholdImage,50,255,THRESH_BINARY);
    blur(thresholdImage,thresholdImage,Size(10,10));
    threshold(thresholdImage,thresholdImage,20,255,THRESH_BINARY);

    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;

    //find contours of filtered image using openCV findContours function
    findContours(thresholdImage,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );// retrieves external contours

    //if contours vector is not empty, we have found some objects
    for(int i=0; i<contours.size(); i++){

        objectBondingRectangle = boundingRect(contours.at(i));

        int objx = objectBondingRectangle.width;
        int objy = objectBondingRectangle.height;

        int xpos = objectBondingRectangle.x+objx/2;
        int ypos = objectBondingRectangle.y+objy/2;

        theObject[0] = xpos;
        theObject[1] = ypos;
        theObject[2] = objx;
        theObject[3] = objy;

        Mat cropedImage = Mat(img,Rect(xpos-objx/2,ypos-objy/2,objx,objy)).clone();
        resize(cropedImage,cropedImage,Size(500,500));

        char imageName[6] = "test";
        imageName[4] = i+48;
        imshow(imageName,cropedImage);
    }

    waitKey(0);
    destroyAllWindows();

    return 0;
}
