//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef __IPC_MASK_HPP
#define __IPC_MASK_HPP


#include "ipc_pipeline.h"


using namespace std;
namespace ipc {
    
 /*------------------------------------------------------------------------ 
    Mask   
 *------------------------------------------------------------------------*/
    
 class Mask : public IPCPipeline
 {
     //variables
 public:
    //  enum MaskParameter
    //  {   
    //      MASK_COUNT
    //  }_active_parameter;
    //  
    // int _param[MASK_COUNT];    

 private:
     // cv::Mat _grayImage;
    
 public:
     Mask(){
         // _active_parameter = TRESHOLD_VALUE;
         _identifyerString = "Mask";
         _type = IP_MASK;
     };
     virtual ~Mask(){};   
    
     virtual void process(){
         _outputImage = cv::Mat::zeros(_inputImage.rows, _inputImage.cols, CV_8UC3);
     }
     
     virtual cv::Mat outputImage(){

         _inputImage.copyTo(_outputImage); //IPCPipeline::outputImage());
         // 
         // if(_inputImage.channels()==3){
         //     cout << "functionality not implemented" << endl;
         //     _outputImage = _inputImage;
         //         
         // } else {
         //     _inputImage.copyTo(_outputImage,IPCPipeline::outputImage());
         // }
          return _outputImage;
     }
 };




}//namespace ipc
#endif //__IPC_MASK_HPP
