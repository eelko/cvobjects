//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef __IPC_TRESHOLD_HPP
#define __IPC_TRESHOLD_HPP


#include "ipc.h"

using namespace std;
namespace ipc {


/*------------------------------------------------------------------------ 
   Threshold   
*------------------------------------------------------------------------*/
    
class Threshold : public IPC
{
    //variables
public:

private:
    cv::Mat _grayImage;
public:
    Threshold(){
        _param["value"] = new DoubleParameter(100,0,255,1,"value");
        _param["type"]  = new DoubleParameter(3,0,4,1,"type");
        
        _identifyerString = "Threshold";
    };
    virtual ~Threshold(){};   


    void process(){
        if(_inputImage.channels()>1){
            cvtColor( _inputImage, _grayImage, CV_RGB2GRAY );
        } else {
            _grayImage = _inputImage;
        }
        cv::threshold( _grayImage, _outputImage, _param["value"]->get(), 255, _param["type"]->get());
        return;
    };

};

}//namespace ipc
#endif //__IPC_TRESHOLD_HPP
