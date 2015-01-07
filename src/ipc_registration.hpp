//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef __IPC_REGISTRATION_HPP
#define __IPC_REGISTRATION_HPP


#include "ipc.h"

using namespace std;
namespace ipc {


/*------------------------------------------------------------------------ 
   Registration
*------------------------------------------------------------------------*/
    
class Registration : public IPC
{
    //variables
public:

private:
    cv::Mat _grayImage;
public:
    Registration(){
        _param["value"] = new DoubleParameter(100,0,255,1,"value");
        _param["type"]  = new DoubleParameter(3,0,4,1,"type");
        
        _identifyerString = "Registration";
    };
    virtual ~Registration(){};   


    void process(){
        if(_referenceImage.channels()>1){
            cvtColor( _referenceImage, _grayImage, CV_RGB2GRAY );
        } else {
            _grayImage = _referenceImage;
        }
        cv::threshold( _grayImage, _outputImage, _param["value"]->get(), 255, _param["type"]->get());
        return;
    };

};

}//namespace ipc
#endif //__IPC_REGISTRATION_HPP
