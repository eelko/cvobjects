//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef __IPC_MORPHOLOGYEX_HPP
#define __IPC_MORPHOLOGYEX_HPP


#include "ipc.h"

using namespace std;
namespace ipc {


/*------------------------------------------------------------------------ 
   MorphologyEx   
*------------------------------------------------------------------------*/
    
class MorphologyEx : public IPC
{

private:
    cv::Mat _grayImage;
    
public:
    MorphologyEx(){
        _param["operation"] = new DoubleParameter(cv::MORPH_OPEN,2,6,1,"operation");
//        _param["iterations"] = new DoubleParameter(1,0,255,1,"iterations");
        _param["size"] = new DoubleParameter(1,0,255,1,"size");
        _param["element"] = new DoubleParameter(0,0,2,1,"element");
        
        _identifyerString = "MorphologyEx";
    };
    virtual ~MorphologyEx(){};   

    void process(){
        if(_inputImage.channels()>1){
            cvtColor( _inputImage, _grayImage, CV_RGB2GRAY );
        } else {
            _grayImage = _inputImage;
        }
        int size = _param["size"]->get();
        cv::Mat element = getStructuringElement( _param["element"]->get() , cv::Size( 2 * size + 1, 2* size +1 ), cv::Point( size , size ) );

        /// Apply the specified morphology operation
        morphologyEx( _inputImage, _outputImage, _param["operation"]->get() , element );
        return;
    };
};


}//namespace ipc
#endif //__IPC_MORPHOLOGYEX_HPP
