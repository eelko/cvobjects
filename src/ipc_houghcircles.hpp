//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef __IPC_HOUGHCIRCLES_HPP
#define __IPC_HOUGHCIRCLES_HPP


#include "ipc.h"

using namespace std;
namespace ipc {


/*------------------------------------------------------------------------ 
   HoughCircles   
*------------------------------------------------------------------------*/
    
class HoughCircles : public IPC
{
    //variables
public:
    enum HoughCirclesParameter
    {   
        HOUGHCIRCLES_MODE,
        HOUGHCIRCLES_MIN_DISTANCE,
        HOUGHCIRCLES_DP,
        HOUGHCIRCLES_UPPER_TRESHOLD_CANNY,
        HOUGHCIRCLES_TRESHOLD_CENTER,
        HOUGHCIRCLES_MIN_RADIUS,
        HOUGHCIRCLES_MAX_RADIUS,
        HOUGHCIRCLES_COUNT
    }_active_parameter;
 
   int _param[HOUGHCIRCLES_COUNT];    

private:
    cv::Mat _grayImage;
    vector<cv::Vec3f> circles;
        
public:
    HoughCircles(){
        _param[HOUGHCIRCLES_MODE                ] = CV_HOUGH_GRADIENT;
        _param[HOUGHCIRCLES_DP                  ] =  1;
        _param[HOUGHCIRCLES_MIN_DISTANCE        ] =  100;
        _param[HOUGHCIRCLES_UPPER_TRESHOLD_CANNY] =  200;
        _param[HOUGHCIRCLES_TRESHOLD_CENTER     ] =  100;
        _param[HOUGHCIRCLES_MIN_RADIUS          ] =  100;
        _param[HOUGHCIRCLES_MAX_RADIUS          ] =  200;
        _active_parameter = HOUGHCIRCLES_MODE;
        _identifyerString = "HoughCircles";
        _type = IP_HOUGHCIRCLES;
    };
    virtual ~HoughCircles(){};   
    
    bool load(const std::string& file_path)
    {
        std::ifstream is(file_path.c_str(), std::ios::binary | std::ios::in);
        if ( !is.is_open() )
            return false;
        for (int i= 0 ; i < HOUGHCIRCLES_COUNT ; i++){
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
    
        for (int i= 0 ; i < HOUGHCIRCLES_COUNT ; i++){
            os.write(reinterpret_cast<const char*>(&_param[i]), std::streamsize(sizeof(int)));
        }
        os.close();
        return true;
    };


    string parameterString(){
        stringstream returnstring;
        returnstring <<
           "(m)ode "                  << _param[HOUGHCIRCLES_MODE                ]<<
           ", (d)p "                  << _param[HOUGHCIRCLES_DP                  ]<<
           ", mi(n) distance "        << _param[HOUGHCIRCLES_MIN_DISTANCE        ]<<
           ", (u)pper threshold canny"<< _param[HOUGHCIRCLES_UPPER_TRESHOLD_CANNY]<<
           ", (c)enter threshold"     << _param[HOUGHCIRCLES_TRESHOLD_CENTER     ]<<
           ", m(i)n radius"           << _param[HOUGHCIRCLES_MIN_RADIUS          ]<<
           ", m(a)x radius"           << _param[HOUGHCIRCLES_MAX_RADIUS          ];
        return returnstring.str();
    };

    void process(){
        if(_inputImage.channels()>1){
            cvtColor( _inputImage, _grayImage, CV_RGB2GRAY );
        } else {
            _grayImage = _inputImage;
        }
        cv::HoughCircles( _grayImage, circles, 
            _param[HOUGHCIRCLES_MODE], 
            _param[HOUGHCIRCLES_DP],
            _param[HOUGHCIRCLES_MIN_DISTANCE        ],
            _param[HOUGHCIRCLES_UPPER_TRESHOLD_CANNY],
            _param[HOUGHCIRCLES_TRESHOLD_CENTER     ],
            _param[HOUGHCIRCLES_MIN_RADIUS          ],
            _param[HOUGHCIRCLES_MAX_RADIUS          ]
        );
        return;
    };

    /// Return the output image of an image processing container.
    cv::Mat outputImage(){

        // cv::Mat drawing = cv::Mat::zeros(_inputImage.size(), CV_8UC3 );
        cv::Mat drawing = _grayImage.clone();
        cvtColor( drawing, drawing, CV_GRAY2RGB );
        for( size_t i = 0; i < circles.size(); i++ )
        {
            cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // circle center
            circle( drawing, center, 3, cv::Scalar(255,0,0), 1, 8, 0 );
            // circle outline
            circle( drawing, center, radius, cv::Scalar(0,0,255), 1, 8, 0 );
         }
     
         _outputImage = drawing;
     
         return _outputImage;
    }



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
    // 
    //         if ( c == 'm' ){ _active_parameter = HOUGHCIRCLES_MODE                ;}
    //         if ( c == 'd' ){ _active_parameter = HOUGHCIRCLES_DP                  ;}
    //         if ( c == 'n' ){ _active_parameter = HOUGHCIRCLES_MIN_DISTANCE        ;}
    //         if ( c == 'u' ){ _active_parameter = HOUGHCIRCLES_UPPER_TRESHOLD_CANNY;}
    //         if ( c == 'c' ){ _active_parameter = HOUGHCIRCLES_TRESHOLD_CENTER     ;}
    //         if ( c == 'i' ){ _active_parameter = HOUGHCIRCLES_MIN_RADIUS          ;}
    //         if ( c == 'a' ){ _active_parameter = HOUGHCIRCLES_MAX_RADIUS          ;}
    // 
    // 
    //         if( c == '+' ){ _param[_active_parameter] ++; }//
    //         if( c == '-' ){ _param[_active_parameter] --; }//
    // 
    //         //limit input
    //         if(_param[HOUGHCIRCLES_MODE]<CV_HOUGH_GRADIENT) _param[HOUGHCIRCLES_MODE] = CV_HOUGH_GRADIENT;
    //         if(_param[HOUGHCIRCLES_MODE]>CV_HOUGH_GRADIENT) _param[HOUGHCIRCLES_MODE] = CV_HOUGH_GRADIENT;
    //         if(_param[HOUGHCIRCLES_DP]<1)                   _param[HOUGHCIRCLES_DP] = 1;
    //         // if(_param[HOUGHCIRCLES_DP]>5)                   _param[HOUGHCIRCLES_DP] = 5;
    //         if(_param[HOUGHCIRCLES_MIN_DISTANCE]<1)         _param[HOUGHCIRCLES_MIN_DISTANCE] = 1;
    //         // if(_param[HOUGHCIRCLES_MIN_DISTANCE]>5)         _param[HOUGHCIRCLES_MIN_DISTANCE] = 5;
    //         if(_param[HOUGHCIRCLES_UPPER_TRESHOLD_CANNY]<1) _param[HOUGHCIRCLES_UPPER_TRESHOLD_CANNY] = 1;
    //         // if(_param[HOUGHCIRCLES_UPPER_TRESHOLD_CANNY]>5) _param[HOUGHCIRCLES_UPPER_TRESHOLD_CANNY] = 5;
    //         if(_param[HOUGHCIRCLES_TRESHOLD_CENTER]<1)      _param[HOUGHCIRCLES_TRESHOLD_CENTER] = 1;
    //         // if(_param[HOUGHCIRCLES_TRESHOLD_CENTER]>5)      _param[HOUGHCIRCLES_TRESHOLD_CENTER] = 5;
    //         if(_param[HOUGHCIRCLES_MIN_RADIUS]<0)           _param[HOUGHCIRCLES_MIN_RADIUS] = 0;
    //         // if(_param[HOUGHCIRCLES_MIN_RADIUS]>5)           _param[HOUGHCIRCLES_MIN_RADIUS] = 5;
    //         if(_param[HOUGHCIRCLES_MAX_RADIUS]<0)           _param[HOUGHCIRCLES_MAX_RADIUS] = 0;
    //         // if(_param[HOUGHCIRCLES_MAX_RADIUS]>5)           _param[HOUGHHOUGHCIRCLES_MAX_RADIUSCIRCLES_METHOD] = 5;
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
#endif //__IPC_HOUGHCIRCLES_HPP
