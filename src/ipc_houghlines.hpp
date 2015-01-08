//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef __IPC_HOUGHLINES_HPP
#define __IPC_HOUGHLINES_HPP


#include "ipc.h"

using namespace std;
namespace ipc {


/*------------------------------------------------------------------------ 
   Houghlines   
*------------------------------------------------------------------------*/
    
class Houghlines : public IPC
{
    //variables
public:

private:
    cv::Mat _grayImage;
public:
    Houghlines(){
        _param["rho"] = new DoubleParameter(1,1,255,1,"rho");
        _param["theta"] = new DoubleParameter(CV_PI/180,0.01,CV_PI,0.1,"theta");
        _param["threshold"] = new DoubleParameter(100,1,255,1,"threshold");
        
        _identifyerString = "Houghlines";
    };
    virtual ~Houghlines(){};   


    void process(){

		/// convert image to grayscale becauuse Houghlines needs a grayscale image
        if(_inputImage.channels()>1){
            cvtColor( _inputImage, _grayImage, CV_RGB2GRAY );
        } else {
            _grayImage = _inputImage;
        }

		_outputImage = cv::Mat::zeros(_inputImage.rows, _inputImage.cols, CV_8UC3);

		//cv::threshold( _grayImage, _outputImage,100, 255, 3);

		/// we need a vector to store the lines
		cv::vector<cv::Vec2f> lines;
		
		/// opencv houghines function
		cv:: HoughLines(_grayImage, lines, _param["rho"]->get(), _param["theta"]->get(), _param["threshold"]->get(), 0, 0 );
		cout << "lines size: " << lines.size() << endl;

		/// display the result by drawing the lines
		for( size_t i = 0; i < lines.size(); i++ )
		{
			float rho = lines[i][0], theta = lines[i][1];
			cv::Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			pt1.x = cvRound(x0 + 1000*(-b));
			pt1.y = cvRound(y0 + 1000*(a));
			pt2.x = cvRound(x0 - 1000*(-b));
			pt2.y = cvRound(y0 - 1000*(a));
			line( _outputImage, pt1, pt2, cv::Scalar(0,0,255), 3, CV_AA);
		}

        return;
    };

};

}//namespace ipc
#endif //__IPC_HOUGHLINES_HPP
