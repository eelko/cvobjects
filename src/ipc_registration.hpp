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
        //if(_referenceImage.channels()>1){
        //    cvtColor( _referenceImage, _grayImage, CV_RGB2GRAY );
        //} else {
        //    _grayImage = _referenceImage;
        //}
        //cv::threshold( _grayImage, _outputImage, _param["value"]->get(), 255, _param["type"]->get());

		//-- Step 3: Matching descriptor vectors using FLANN matcher
//  FlannBasedMatcher matcher;  
//  std::vector< vector<DMatch>  > matches;
//  matcher.knnMatch( descriptors_object, descriptors_scene, matches, 2 );    
//  vector< DMatch > good_matches;
//  good_matches.reserve(matches.size());  
//
//  for (size_t i = 0; i < matches.size(); ++i)
//  { 
//      if (matches[i].size() < 2)
//                  continue;
//
//      const DMatch &m1 = matches[i][0];
//      const DMatch &m2 = matches[i][1];
//
//      if(m1.distance <= nndrRatio * m2.distance)        
//      good_matches.push_back(m1);     
//  }
//
//  //If there are at least 7 good matches, then object has been found
//  if( (good_matches.size() >=7))
//  { 
//    cout << "OBJECT FOUND!" << endl;
//
//    std::vector<Point2f> obj;
//    std::vector<Point2f> scene;
//
//    for( unsigned int i = 0; i < good_matches.size(); i++ )
//    {
//        //-- Get the keypoints from the good matches
//        obj.push_back( keypointsO[ good_matches[i].queryIdx ].pt );
//        scene.push_back( keypointsS[ good_matches[i].trainIdx ].pt );
//    }
//
//    Mat H = findHomography( obj, scene, CV_RANSAC );
//
//
//
//    //-- Get the corners from the image_1 ( the object to be "detected" )
//    std::vector<Point2f> obj_corners(4);
//    obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( objectP.cols, 0 );
//    obj_corners[2] = cvPoint( objectP.cols, objectP.rows ); obj_corners[3] = cvPoint( 0, objectP.rows );
//    std::vector<Point2f> scene_corners(4);
//
//    perspectiveTransform( obj_corners, scene_corners, H);
//
//
//    //-- Draw lines between the corners (the mapped object in the scene - image_2 ) 
//    line( outImg, scene_corners[0] , scene_corners[1], color, 2 ); //TOP line
//    line( outImg, scene_corners[1] , scene_corners[2], color, 2 );
//    line( outImg, scene_corners[2] , scene_corners[3], color, 2 );
//    line( outImg, scene_corners[3] , scene_corners[0] , color, 2 ); 
//    objectFound=true;
//  }
//else
//{
//  cout << "OBJECT NOT FOUND" << endl;
//}
//

        return;
    };

};

}//namespace ipc
#endif //__IPC_REGISTRATION_HPP
