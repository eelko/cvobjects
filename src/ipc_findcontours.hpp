//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef __IPC_FINDCONTOURS_HPP
#define __IPC_FINDCONTOURS_HPP


#include "ipc.h"
#include <math.h>

using namespace std;
namespace ipc {


/*------------------------------------------------------------------------ 
   FindContours   
*------------------------------------------------------------------------*/
    
class FindContours : public IPC
{
    //variables
public:
    enum FindContoursParameter
    {   
        FINDCONTOURS_MODE,
        FINDCONTOURS_METHOD,
        FINDCONTOURS_COUNT
    }_active_parameter;
 
   int _param[FINDCONTOURS_COUNT];    

private:
    cv::Mat _grayImage;
    vector<vector<cv::Point> > contours;
    vector<cv::Vec4i> hierarchy;
        
public:
    FindContours(){
        _param[FINDCONTOURS_MODE] = CV_RETR_TREE;
        _param[FINDCONTOURS_METHOD]  = CV_CHAIN_APPROX_SIMPLE;
        _active_parameter = FINDCONTOURS_MODE;
        _identifyerString = "FindContours";
        _type = IP_CONTOURS;
    };
    virtual ~FindContours(){};   
    
    bool load(const std::string& file_path)
    {
        std::ifstream is(file_path.c_str(), std::ios::binary | std::ios::in);
        if ( !is.is_open() )
            return false;
        for (int i= 0 ; i < FINDCONTOURS_COUNT ; i++){
            is.read(reinterpret_cast<char*>(&_param[i]), std::streamsize(sizeof(int)));
        }
        is.close();
        return true;
    };

    bool save(const std::string& file_path)
    {
        std::ofstream os(file_path.c_str(), std::ios::binary | std::ios::out);
        if ( !os.is_open() )
        	return false;
    
        for (int i= 0 ; i < FINDCONTOURS_COUNT ; i++){
            os.write(reinterpret_cast<const char*>(&_param[i]), std::streamsize(sizeof(int)));
        }
        os.close();
        return true;
    };


    string parameterString(){
        stringstream returnstring;
        returnstring <<
             "FindContours (m)ode " <<    _param[FINDCONTOURS_MODE ]<<     
             ", FindContours m(e)thod " <<   _param[FINDCONTOURS_METHOD];     
        return returnstring.str();
    };

    void process(){
        if(_inputImage.channels()>1){
            cvtColor( _inputImage, _grayImage, CV_RGB2GRAY );
        } else {
            _grayImage = _inputImage;
        }
        findContours( _grayImage, contours, hierarchy, _param[FINDCONTOURS_MODE], _param[FINDCONTOURS_METHOD], cv::Point(0, 0) );

        return;
    };
    
    
    // void extra(){
    // 
    // }

    /// Return the output image of an image processing container.
    cv::Mat outputImage(){
        //now produce output image
        cv::RNG rng(12345);
        cv::Mat drawing = cv::Mat::zeros(_inputImage.size(), CV_8UC3 );

        for( int i = 0; i< contours.size(); i++ )
           {
             cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
             drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point() );
           }
       
           _outputImage = drawing;

        return _outputImage;
    }


    // 
    // void interactiveTune(int posX, int posY)
    // {
    //     cv::namedWindow( _identifyerString, cv::WINDOW_AUTOSIZE );
    //     cv::moveWindow(_identifyerString, posX , posY );
    // 
    //     for(;;)
    //     {
    //         int c = (char)cv::waitKey( IPC_wait_key_period );
    // 
    //         if( c == 27 ){ break; }
    //         if( c == 'm' ){ _active_parameter =FINDCONTOURS_MODE ;}
    //         if( c == 'e' ){ _active_parameter =FINDCONTOURS_METHOD;}
    // 
    //         if( c == '+' ){ _param[_active_parameter] ++; }//
    //         if( c == '-' ){ _param[_active_parameter] --; }//
    // 
    //         //limit input
    //         if(_param[FINDCONTOURS_MODE]<0) _param[FINDCONTOURS_MODE] = 0;
    //         if(_param[FINDCONTOURS_MODE]>3) _param[FINDCONTOURS_MODE] = 3;
    //         if(_param[FINDCONTOURS_METHOD]<1) _param[FINDCONTOURS_METHOD] = 1;
    //         if(_param[FINDCONTOURS_METHOD]>5) _param[FINDCONTOURS_METHOD] = 5;
    // 
    //         cout << "\r" << parameterString() << "            " <<flush;
    //         process();
    // 
    //         if (_outputImage.data)
    //             cv::imshow(_identifyerString, outputImage());
    //       }
    //       //cleanup
    //       cout << endl;
    //        cv::destroyWindow(_identifyerString);
    //       return;
    // }
};




}//namespace ipc
#endif //__IPC_FINDCONTOURS_HPP
