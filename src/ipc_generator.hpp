//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef __IMAGEPROCESSINGCONTAINER_FACTORY_HPP__
#define __IMAGEPROCESSINGCONTAINER_FACTORY_HPP__

//#include "ipc.h"
 // #include "ipc_pipeline.hpp"
 // #include "ipc_laplacian.hpp"
 // #include "ipc_gaussianblurisotropic.hpp"
 // #include "ipc_colorsegmenthsv.hpp"
 // #include "ipc_threshold.hpp"
 // #include "ipc_findcontours.hpp"
 // #include "ipc_houghcircles.hpp"
//#include "ipc_mask.hpp"


namespace ipc {
class IPCGen
{
    //variables
public:

    static IPC* createIPC(string identifyerString){
                if ( identifyerString == "Laplacian")  return new Laplacian();
                if ( identifyerString == "GaussianBlurIsotropic")  return (IPC*) new GaussianBlurIsotropic();
                //if ( identifyerString == "?")  return new ColorSegmentHSV();
                if ( identifyerString == "Threshold")  return new Threshold();
                //if ( identifyerString == "?")  return new FindContours();
                //if ( identifyerString == "?")  return new HoughCircles();
                if ( identifyerString == "MorphologyEx")  return new MorphologyEx();
                //if ( identifyerString == "?")  return (IPC*) new Mask();
        return NULL;
    }


};
}//namespace ipc

#endif //__IMAGEPROCESSINGCONTAINER_FACTORY_HPP__
