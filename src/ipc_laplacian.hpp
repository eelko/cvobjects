//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef __IPC_LAPLACIAN_HPP
#define __IPC_LAPLACIAN_HPP


#include "ipc.h"

using namespace std;
namespace ipc {


/*------------------------------------------------------------------------ 
   Laplacian   
*------------------------------------------------------------------------*/
    
class Laplacian : public IPC
{
private:
    cv::Mat _grayImage;
    cv::Mat _laplaceImage;
    
public:
    Laplacian(){
        _param["kernelSize"] = new DoubleParameter(1,1,15,1,"kernelSize");
        _param["kernelScale"] = new DoubleParameter(1,1,50,1,"kernelScale");
        _param["delta"] = new DoubleParameter(1,0,255,1,"delta");
        _param["borderType"] = new DoubleParameter(cv::BORDER_DEFAULT,cv::BORDER_DEFAULT,cv::BORDER_DEFAULT,1,"borderType");
        
        _identifyerString = "Laplacian";
    };
    virtual ~Laplacian(){};   
    

    void process(){
        if(_inputImage.channels()>1){
            cvtColor( _inputImage, _grayImage, CV_RGB2GRAY );
        } else {
            _grayImage = _inputImage;
        }
    
        cv::Laplacian( _grayImage, _outputImage, CV_16S, 2*_param["kernelSize"]->get()+1, _param["kernelScale"]->get(), _param["delta"]->get(), _param["borderType"]->get() );
        convertScaleAbs( _outputImage, _outputImage ); //we rescale to absolute integers
        return;
    };
};

}//namespace ipc
#endif //__IPC_LAPLACIAN_HPP
