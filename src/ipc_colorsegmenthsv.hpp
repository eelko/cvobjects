//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef __IPC_COLORSEGMENTHSV_HPP
#define __IPC_COLORSEGMENTHSV_HPP

#include "ipc.h"

using namespace std;
namespace ipc {

/*------------------------------------------------------------------------ 
    ColorSegmentHSV   
*------------------------------------------------------------------------*/

class ColorSegmentHSV : public IPC
{
private:
    cv::Mat _imageHSV;
    std::vector<cv::Mat> _bin;
    std::vector<cv::Mat> _composite;
    enum ColorRange   {   CR_BLUE, CR_GREEN, CR_RED, CR_COUNT };
    

public:
    ColorSegmentHSV(){
        _identifyerString = "ColorSegmentHSV";

        _bin.resize(3);
        _composite.resize(3);

        _param["RED_HUE_LOWER_LIMIT"]   = new DoubleParameter(0  ,0,255,1,"RED_HUE_LOWER_LIMIT");
        _param["RED_HUE_UPPER_LIMIT"]   = new DoubleParameter(34 ,0,255,1,"RED_HUE_UPPER_LIMIT");
        _param["RED_VAL_LOWER_LIMIT"]   = new DoubleParameter(0  ,0,255,1,"RED_VAL_LOWER_LIMIT");
        _param["RED_VAL_HIGHER_LIMIT"]  = new DoubleParameter(255,0,255,1,"RED_VAL_HIGHER_LIMIT");
        _param["RED_SAT_LOWER_LIMIT"]   = new DoubleParameter(0  ,0,255,1,"RED_SAT_LOWER_LIMIT");
        _param["RED_SAT_HIGHER_LIMIT"]  = new DoubleParameter(255,0,255,1,"RED_SAT_HIGHER_LIMIT");

        _param["GREEN_HUE_LOWER_LIMIT"]   = new DoubleParameter(50 ,0,255,1,"GREEN_HUE_LOWER_LIMIT");
        _param["GREEN_HUE_UPPER_LIMIT"]   = new DoubleParameter(90 ,0,255,1,"GREEN_HUE_UPPER_LIMIT");
        _param["GREEN_VAL_LOWER_LIMIT"]   = new DoubleParameter(0  ,0,255,1,"GREEN_VAL_LOWER_LIMIT");
        _param["GREEN_VAL_HIGHER_LIMIT"]  = new DoubleParameter(255,0,255,1,"GREEN_VAL_HIGHER_LIMIT");
        _param["GREEN_SAT_LOWER_LIMIT"]   = new DoubleParameter(0  ,0,255,1,"GREEN_SAT_LOWER_LIMIT");
        _param["GREEN_SAT_HIGHER_LIMIT"]  = new DoubleParameter(255,0,255,1,"GREEN_SAT_HIGHER_LIMIT");

        _param["BLUE_HUE_LOWER_LIMIT"]   = new DoubleParameter(120,0,255,1,"BLUE_HUE_LOWER_LIMIT");
        _param["BLUE_HUE_UPPER_LIMIT"]   = new DoubleParameter(180,0,255,1,"BLUE_HUE_UPPER_LIMIT");
        _param["BLUE_VAL_LOWER_LIMIT"]   = new DoubleParameter(0  ,0,255,1,"BLUE_VAL_LOWER_LIMIT");
        _param["BLUE_VAL_HIGHER_LIMIT"]  = new DoubleParameter(255,0,255,1,"BLUE_VAL_HIGHER_LIMIT");
        _param["BLUE_SAT_LOWER_LIMIT"]   = new DoubleParameter(0  ,0,255,1,"BLUE_SAT_LOWER_LIMIT");
        _param["BLUE_SAT_HIGHER_LIMIT"]  = new DoubleParameter(255,0,255,1,"BLUE_SAT_HIGHER_LIMIT");

        _param["_MASK_MODE"]  = new DoubleParameter(0,0,1,1,"_MASK_MODE");

    };

    virtual ~ColorSegmentHSV(){} ;   


    /// Set the input image of an image processing container.
    void inputImage(cv::Mat inputImage){
            _inputImage = inputImage;
            _bin[CR_RED] = cv::Mat(_inputImage.rows, _inputImage.cols, CV_8UC1);
            _bin[CR_GREEN] = cv::Mat(_inputImage.rows, _inputImage.cols, CV_8UC1);
            _bin[CR_BLUE] = cv::Mat(_inputImage.rows, _inputImage.cols, CV_8UC1);

            _composite[CR_RED] = cv::Mat(_inputImage.rows, _inputImage.cols, CV_8UC1);
            _composite[CR_GREEN] = cv::Mat(_inputImage.rows, _inputImage.cols, CV_8UC1);
            _composite[CR_BLUE] = cv::Mat(_inputImage.rows, _inputImage.cols, CV_8UC1);

        
            return;
    }


    void process(){
    
        if(_inputImage.channels()==3){
    //        _outputImage = cv::Mat(_inputImage.rows, _inputImage.cols, CV_8UC3);
            cvtColor(_inputImage, _imageHSV, CV_BGR2HSV);
            // _inputImage.copyTo(_imageHSV);
           //segment three (preselected) color ranges
           cv::inRange(        _imageHSV,
                               cv::Scalar(_param["RED_HUE_LOWER_LIMIT"]->get() ,_param["RED_SAT_LOWER_LIMIT"]->get() ,_param["RED_VAL_LOWER_LIMIT"]->get()),
                               cv::Scalar(_param["RED_HUE_UPPER_LIMIT"]->get() ,_param["RED_SAT_HIGHER_LIMIT"]->get(),_param["RED_VAL_HIGHER_LIMIT"]->get()),
                               _bin[CR_RED]);

           cv::inRange(        _imageHSV,
                               cv::Scalar(_param["GREEN_HUE_LOWER_LIMIT"]->get() ,_param["GREEN_SAT_LOWER_LIMIT"]->get() ,_param["GREEN_VAL_LOWER_LIMIT"]->get()),
                               cv::Scalar(_param["GREEN_HUE_UPPER_LIMIT"]->get() ,_param["GREEN_SAT_HIGHER_LIMIT"]->get(),_param["GREEN_VAL_HIGHER_LIMIT"]->get()),
                               _bin[CR_GREEN]);

           cv::inRange(        _imageHSV,
                               cv::Scalar(_param["BLUE_HUE_LOWER_LIMIT"]->get() ,_param["BLUE_SAT_LOWER_LIMIT"]->get() ,_param["BLUE_VAL_LOWER_LIMIT"]->get()),
                               cv::Scalar(_param["BLUE_HUE_UPPER_LIMIT"]->get() ,_param["BLUE_SAT_HIGHER_LIMIT"]->get(),_param["BLUE_VAL_HIGHER_LIMIT"]->get()),
                               _bin[CR_BLUE]);

           if(_param["_MASK_MODE"]->get()==0){
               cv::merge(_bin, _outputImage);
           }
           else if(_param["_MASK_MODE"]->get()==1){
               //reset output image to prevent masking effect
               _outputImage = cv::Mat::zeros(_inputImage.rows, _inputImage.cols, CV_8UC3);
               //create a ...
               cv::Mat _grayImage;
               cvtColor( _inputImage, _grayImage, CV_RGB2GRAY );
           
               // 
               // _grayImage.copyTo(_composite[CR_RED],_bin[CR_RED]);
               // _grayImage.copyTo(_composite[CR_GREEN],_bin[CR_GREEN]);
               // _grayImage.copyTo(_composite[CR_BLUE],_bin[CR_BLUE]);
               // 
               // cv::merge(_composite, _outputImage);

              cv::Mat maskImage(_inputImage.rows, _inputImage.cols, CV_8UC1);
              maskImage = _bin[CR_RED]+_bin[CR_GREEN]+_bin[CR_BLUE];
             _grayImage.copyTo(_outputImage,maskImage);
            }

        } else {
            cerr << _identifyerString << ".process() only works on color images!" << endl;
        }
                        
        return;
    };


};



}//namespace ipc
#endif //__IPC_COLORSEGMENTHSV_HPP
