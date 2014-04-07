//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef __IPC_COLORSEGMENTHSV_HPP
#define __IPC_COLORSEGMENTHSV_HPP

#include "ipc.h"

using namespace std;
namespace ipc {

/*------------------------------------------------------------------------ 
    ColorSegmentHSV   
*------------------------------------------------------------------------*/

class ColorSegmentHSV : public IPC
{
public:
    enum ColorRange   {   CR_BLUE, CR_GREEN, CR_RED, CR_COUNT };
    enum ColorType    {   CT_HUE, CT_SATURATION, CT_VALUE, CT_COUNT };
    enum ColorSelect  {   CS_LOWER_LIMIT, CS_UPPER_LIMIT, CS_COUNT };

    enum ColorSegmentMode 
    {
        //COLORSEGMENTMODE_BINARY, 
        COLORSEGMENTMODE_TRICOLOR, 
        COLORSEGMENTMODE_MASK,
        COLORSEGMENTMODE_COUNT
    };
    
    enum ColorSegmentParameter
    {   
        COLORSEGMENT_RANGE,
        COLORSEGMENT_MODE,
//        COLORSEGMENT_TYPE,
        COLORSEGMENT_COUNT
    }_active_parameter;
 
   int _param[COLORSEGMENT_COUNT];    
    
    
    ColorRange _active_range; //we have three ranges we call red, green and blue 
    ColorType _active_type; //differentiate between Hue, Saturation and Value
    ColorSelect _active_select; //upper or lower limit

    typedef unsigned char LIM_TYPE;
    LIM_TYPE _lim[CR_COUNT][CT_COUNT][CS_COUNT] ;
private:
    cv::Mat _imageHSV;
    std::vector<cv::Mat> _bin;
    std::vector<cv::Mat> _composite;
    

public:
    ColorSegmentHSV():_active_range(CR_RED),_active_type(CT_HUE),_active_select(CS_UPPER_LIMIT){
        _identifyerString = "ColorSegmentHSV";
        _type = IP_COLORSEGMENTATION;
        
        _active_parameter = COLORSEGMENT_RANGE;
        _bin.resize(CR_COUNT);
        _composite.resize(CR_COUNT);

        _lim[CR_RED]      [CT_HUE]       [CS_LOWER_LIMIT] =  0 ;
        _lim[CR_RED]      [CT_HUE]       [CS_UPPER_LIMIT] =  34 ;
        _lim[CR_RED]      [CT_SATURATION][CS_LOWER_LIMIT] =  158 ;
        _lim[CR_RED]      [CT_SATURATION][CS_UPPER_LIMIT] =  255;
        _lim[CR_RED]      [CT_VALUE]     [CS_LOWER_LIMIT] =  100 ;
        _lim[CR_RED]      [CT_VALUE]     [CS_UPPER_LIMIT] =  223;
        _lim[CR_GREEN]    [CT_HUE]       [CS_LOWER_LIMIT] =  50 ;
        _lim[CR_GREEN]    [CT_HUE]       [CS_UPPER_LIMIT] =  90 ;
        _lim[CR_GREEN]    [CT_SATURATION][CS_LOWER_LIMIT] =  0 ;
        _lim[CR_GREEN]    [CT_SATURATION][CS_UPPER_LIMIT] =  255;
        _lim[CR_GREEN]    [CT_VALUE]     [CS_LOWER_LIMIT] =  0 ;
        _lim[CR_GREEN]    [CT_VALUE]     [CS_UPPER_LIMIT] =  255;
        _lim[CR_BLUE]     [CT_HUE]       [CS_LOWER_LIMIT] =  120 ;
        _lim[CR_BLUE]     [CT_HUE]       [CS_UPPER_LIMIT] =  180 ;
        _lim[CR_BLUE]     [CT_SATURATION][CS_LOWER_LIMIT] =  0 ;
        _lim[CR_BLUE]     [CT_SATURATION][CS_UPPER_LIMIT] =  255 ;
        _lim[CR_BLUE]     [CT_VALUE]     [CS_LOWER_LIMIT] =  0 ;
        _lim[CR_BLUE]     [CT_VALUE]     [CS_UPPER_LIMIT] =  255 ;
        
        _param[COLORSEGMENT_MODE]  = COLORSEGMENTMODE_TRICOLOR;

    };

    virtual ~ColorSegmentHSV(){} ;   


    /// Set the input image of an image processing container.
    void inputImage(cv::Mat inputImage){
            _inputImage = inputImage;
            _bin[CR_RED] = cv::Mat(_inputImage.rows, _inputImage.cols, CV_8UC1);
            _bin[CR_GREEN] = cv::Mat(_inputImage.rows, _inputImage.cols, CV_8UC1);
            _bin[CR_BLUE] = cv::Mat(_inputImage.rows, _inputImage.cols, CV_8UC1);

            _composite[CR_RED] = cv::Mat(_inputImage.rows, _inputImage.cols, CV_8UC1);
            _composite[CR_GREEN] = cv::Mat(_inputImage.rows, _inputImage.cols, CV_8UC1);
            _composite[CR_BLUE] = cv::Mat(_inputImage.rows, _inputImage.cols, CV_8UC1);

        
            return;
    }

    bool load(const std::string& file_path)
    {
        std::ifstream is(file_path.c_str(), std::ios::binary | std::ios::in);
        if ( !is.is_open() )
            return false;
        for (int r = 0; r < CR_COUNT ; r++){
            for (int t = 0 ; t < CT_COUNT ; t++){
                for (int s= 0 ; s < CS_COUNT ; s++){
                    is.read(reinterpret_cast<char*>(&_lim[r][t][s]), std::streamsize(sizeof(LIM_TYPE)));
                }
            }
        }
        for (int i= 0 ; i < COLORSEGMENT_COUNT ; i++){
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
    
        for (int r = 0; r < CR_COUNT ; r++){
            for (int t = 0 ; t < CT_COUNT ; t++){
                for (int s= 0 ; s < CS_COUNT ; s++){
                    os.write(reinterpret_cast<const char*>(&_lim[r][t][s]), std::streamsize(sizeof(LIM_TYPE)));
                }
            }
        }
        for (int i= 0 ; i < COLORSEGMENT_COUNT ; i++){
            os.write(reinterpret_cast<const char*>(&_param[i]), std::streamsize(sizeof(int)));
        }
        os.close();
        return true;
    };


    string parameterString(){
        stringstream returnstring;
        returnstring << "(rhl)" << (int)_lim[CR_RED]      [CT_HUE]       [CS_LOWER_LIMIT]     << " "  ;
        returnstring << "(rhu)" << (int)_lim[CR_RED]      [CT_HUE]       [CS_UPPER_LIMIT]     << " |" ;
        returnstring << "(rsl)" << (int)_lim[CR_RED]      [CT_SATURATION][CS_LOWER_LIMIT]     << " "  ;
        returnstring << "(rsu)" << (int)_lim[CR_RED]      [CT_SATURATION][CS_UPPER_LIMIT]     << " |" ;
        returnstring << "(rvl)" << (int)_lim[CR_RED]      [CT_VALUE]     [CS_LOWER_LIMIT]     << " "  ;
        returnstring << "(rvu)" << (int)_lim[CR_RED]      [CT_VALUE]     [CS_UPPER_LIMIT]     << " ||";
        returnstring << "(ghl)" << (int)_lim[CR_GREEN]    [CT_HUE]       [CS_LOWER_LIMIT]     << " "  ;
        returnstring << "(ghu)" << (int)_lim[CR_GREEN]    [CT_HUE]       [CS_UPPER_LIMIT]     << " |" ;
        returnstring << "(gsl)" << (int)_lim[CR_GREEN]    [CT_SATURATION][CS_LOWER_LIMIT]     << " "  ;
        returnstring << "(gsu)" << (int)_lim[CR_GREEN]    [CT_SATURATION][CS_UPPER_LIMIT]     << " |" ;
        returnstring << "(gvl)" << (int)_lim[CR_GREEN]    [CT_VALUE]     [CS_LOWER_LIMIT]     << " "  ;
        returnstring << "(gvu)" << (int)_lim[CR_GREEN]    [CT_VALUE]     [CS_UPPER_LIMIT]     << " ||";
        returnstring << "(bhl)" << (int)_lim[CR_BLUE]     [CT_HUE]       [CS_LOWER_LIMIT]     << " "  ;
        returnstring << "(bhu)" << (int)_lim[CR_BLUE]     [CT_HUE]       [CS_UPPER_LIMIT]     << " |" ;
        returnstring << "(bsl)" << (int)_lim[CR_BLUE]     [CT_SATURATION][CS_LOWER_LIMIT]     << " "  ;
        returnstring << "(bsu)" << (int)_lim[CR_BLUE]     [CT_SATURATION][CS_UPPER_LIMIT]     << " |" ;
        returnstring << "(bvl)" << (int)_lim[CR_BLUE]     [CT_VALUE]     [CS_LOWER_LIMIT]     << " "  ;
        returnstring << "(bvu)" << (int)_lim[CR_BLUE]     [CT_VALUE]     [CS_UPPER_LIMIT]     << " "  ;
        returnstring << "(m)ode" << _param[COLORSEGMENT_MODE];
        return returnstring.str();
    
    };

    void process(){
    
        if(_inputImage.channels()==3){
    //        _outputImage = cv::Mat(_inputImage.rows, _inputImage.cols, CV_8UC3);
            cvtColor(_inputImage, _imageHSV, CV_BGR2HSV);
            // _inputImage.copyTo(_imageHSV);
           //segment three (preselected) color ranges
           cv::inRange(        _imageHSV,
                               cv::Scalar(_lim[CR_RED][CT_HUE][CS_LOWER_LIMIT],_lim[CR_RED][CT_SATURATION][CS_LOWER_LIMIT],_lim[CR_RED][CT_VALUE][CS_LOWER_LIMIT]),
                               cv::Scalar(_lim[CR_RED][CT_HUE][CS_UPPER_LIMIT],_lim[CR_RED][CT_SATURATION][CS_UPPER_LIMIT],_lim[CR_RED][CT_VALUE][CS_UPPER_LIMIT]),
                               _bin[CR_RED]);

           cv::inRange(        _imageHSV,
                               cv::Scalar(_lim[CR_GREEN][CT_HUE][CS_LOWER_LIMIT],_lim[CR_GREEN][CT_SATURATION][CS_LOWER_LIMIT],_lim[CR_GREEN][CT_VALUE][CS_LOWER_LIMIT]),
                               cv::Scalar(_lim[CR_GREEN][CT_HUE][CS_UPPER_LIMIT],_lim[CR_GREEN][CT_SATURATION][CS_UPPER_LIMIT],_lim[CR_GREEN][CT_VALUE][CS_UPPER_LIMIT]),
                               _bin[CR_GREEN]);

           cv::inRange(        _imageHSV,
                               cv::Scalar(_lim[CR_BLUE][CT_HUE][CS_LOWER_LIMIT],_lim[CR_BLUE][CT_SATURATION][CS_LOWER_LIMIT],_lim[CR_BLUE][CT_VALUE][CS_LOWER_LIMIT]),
                               cv::Scalar(_lim[CR_BLUE][CT_HUE][CS_UPPER_LIMIT],_lim[CR_BLUE][CT_SATURATION][CS_UPPER_LIMIT],_lim[CR_BLUE][CT_VALUE][CS_UPPER_LIMIT]),
                               _bin[CR_BLUE]);

           if(_param[COLORSEGMENT_MODE]==COLORSEGMENTMODE_TRICOLOR){
               cv::merge(_bin, _outputImage);
           }
           else if(_param[COLORSEGMENT_MODE]==COLORSEGMENTMODE_MASK){
               cv::Mat _grayImage;
               cvtColor( _inputImage, _grayImage, CV_RGB2GRAY );
           
               // 
               // _grayImage.copyTo(_composite[CR_RED],_bin[CR_RED]);
               // _grayImage.copyTo(_composite[CR_GREEN],_bin[CR_GREEN]);
               // _grayImage.copyTo(_composite[CR_BLUE],_bin[CR_BLUE]);
               // 
               // cv::merge(_composite, _outputImage);

              cv::Mat maskImage(_inputImage.rows, _inputImage.cols, CV_8UC1);
              maskImage = _bin[CR_RED]+_bin[CR_GREEN]+_bin[CR_BLUE];
             _grayImage.copyTo(_outputImage,maskImage);
            }

        } else {
            cerr << _identifyerString << ".process() only works on color images!" << endl;
        }
                        
        return;
    };

    void process(ColorRange range){
        _outputImage = cv::Mat(_imageHSV.rows, _imageHSV.cols, CV_8UC1);
        cvtColor(_inputImage, _imageHSV, CV_BGR2HSV);
        //segment three (preselected) color ranges
        cv::inRange(        _imageHSV,
                            cv::Scalar(_lim[range][CT_HUE][CS_LOWER_LIMIT],_lim[range][CT_SATURATION][CS_LOWER_LIMIT],_lim[range][CT_VALUE][CS_LOWER_LIMIT]),
                            cv::Scalar(_lim[range][CT_HUE][CS_UPPER_LIMIT],_lim[range][CT_SATURATION][CS_UPPER_LIMIT],_lim[range][CT_VALUE][CS_UPPER_LIMIT]),
                            _bin[range]);

        _outputImage =  _bin[range];
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
        //         if (c == 'm' ){ _active_parameter = COLORSEGMENT_MODE;}
        //         if( c == 'r' ){ _active_parameter = COLORSEGMENT_RANGE; _active_range = CR_RED;}//r - red
        //         if( c == 'g' ){ _active_parameter = COLORSEGMENT_RANGE; _active_range = CR_GREEN;}//g - green
        //         if( c == 'b' ){ _active_parameter = COLORSEGMENT_RANGE; _active_range = CR_BLUE;}//b - blue
        //         if( c == 'h' ){ _active_parameter = COLORSEGMENT_RANGE; _active_type = CT_HUE; }//
        //         if( c == 's' ){ _active_parameter = COLORSEGMENT_RANGE; _active_type = CT_SATURATION; }//
        //         if( c == 'v' ){ _active_parameter = COLORSEGMENT_RANGE; _active_type = CT_VALUE; }//
        //         if( c == 'u' ){ _active_parameter = COLORSEGMENT_RANGE; _active_select = CS_UPPER_LIMIT; }//
        //         if( c == 'l' ){ _active_parameter = COLORSEGMENT_RANGE; _active_select = CS_LOWER_LIMIT; }//
        //         if( c == '+' ){ 
        //             if(_active_parameter == COLORSEGMENT_RANGE) 
        //                 _lim[_active_range][_active_type][_active_select] ++; 
        //             else
        //                 _param[_active_parameter] ++; 
        //         }//
        //         
        //         if( c == '-' ){ 
        //             if(_active_parameter == COLORSEGMENT_RANGE) 
        //                 _lim[_active_range][_active_type][_active_select] --; 
        //             else
        //                 _param[_active_parameter] --;
        //         } 
        //     
        //         if(_param[COLORSEGMENT_MODE]<0) _param[COLORSEGMENT_MODE] = 0;
        //         if(_param[COLORSEGMENT_MODE]>=COLORSEGMENTMODE_COUNT) _param[COLORSEGMENT_MODE] = COLORSEGMENTMODE_COUNT-1;
        //     
        // 
        //         cout << "\r" << parameterString() << "            " <<flush;
        //         process();
        // 
        //         if (_outputImage.data)
        //             cv::imshow(_identifyerString, _outputImage);
        //       }
        //       //cleanup
        //       cout << endl;
        //        cv::destroyWindow(_identifyerString);
        //       return;
        // }
        // 

};



}//namespace ipc
#endif //__IPC_COLORSEGMENTHSV_HPP
