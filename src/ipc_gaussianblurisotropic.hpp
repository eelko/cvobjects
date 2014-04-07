//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef __IPC_GAUSSIANBLURISOTROPIC_HPP
#define __IPC_GAUSSIANBLURISOTROPIC_HPP

#include "ipc.h"

using namespace std;
namespace ipc {

/*------------------------------------------------------------------------ 
    GaussianBlurIsotropic   
*------------------------------------------------------------------------*/
    
class GaussianBlurIsotropic : public IPC
{
public:
    GaussianBlurIsotropic(){
        _identifyerString = "GaussianBlurIsotropic";
        _type = IP_BLUR;
        
        _param["kernelSize"] = new DoubleParameter(10,1,255,1,"kernelSize");
        _param["standardDeviation"]  = new DoubleParameter(0,0,100,0.1,"standardDeviation");
    
    };
    virtual ~GaussianBlurIsotropic(){};   
    
    void process(){
        cv::GaussianBlur(_inputImage, _outputImage, cv::Size((2*_param["kernelSize"]->get())+1,(2*_param["kernelSize"]->get())+1), _param["standardDeviation"]->get(), 0, cv::BORDER_DEFAULT );
        return;
    };

};

}//namespace ipc
#endif //__IPC_GAUSSIANBLURISOTROPIC_HPP
