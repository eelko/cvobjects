//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef __IPC_CANNY_HPP
#define __IPC_CANNY_HPP


#include "ipc.h"

using namespace std;
namespace ipc {


/*------------------------------------------------------------------------ 
   Threshold   
*------------------------------------------------------------------------*/
    
class Canny : public IPC
{
    //variables
public:

private:
    cv::Mat _grayImage;
public:
    Canny(){
        _param["treshold"] = new DoubleParameter(1,1,100,1,"threshold");
        _param["ratio"] = new DoubleParameter(3,1,5,0.5,"ratio");
        _param["kernel_size"] = new DoubleParameter(3,3,3,1,"kernel_size");
        
        _identifyerString = "Canny";
    };
    virtual ~Canny(){};   


    void process(){
        if(_inputImage.channels()>1){
            cvtColor( _inputImage, _grayImage, CV_RGB2GRAY );
        } else {
            _grayImage = _inputImage;
        }
        //cv::threshold( _grayImage, _outputImage, _param["value"]->get(), 255, _param["type"]->get());
		cv::Canny( _grayImage, _outputImage, _param["treshold"]->get(), _param["treshold"]->get()*_param["ratio"]->get(), _param["kernel_size"]->get() );
		return;
    };

};

}//namespace ipc
#endif //__IPC_CANNY_HPP
