//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef __IMAGEPROCESSINGCONTAINER__HPP__
#define __IMAGEPROCESSINGCONTAINER__HPP__

#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <fstream>
#include "log.h"
#include "tinyxml.h"
#include "parameter.hpp"

const int IPC_wait_key_period = 10;


using namespace std;
namespace ipc {




    
///Base Image Processing Container Class
/**
IPC is the base class for all image processing containers and implements the 
standard interface so containers can be unifomly used in pipelines and other 
architectural constructions.
*/

class IPC
{
    //variables
public:
    std::map<std::string, DoubleParameter*> _param; /// STL Map containing all the parameters of the IPC referenced by their indetifying string
protected:
    string _identifyerString;       ///< String identifying object type, used for file names and reporting.
    cv::Mat _inputImage;            ///< The standard location of the incomming images.
    cv::Mat _outputImage;           ///< The standard location of the outgoing images.
    
    //methods
public:
    IPC():_identifyerString("IPC"){}; //<We fill the identifyer enum and string during construction.

    virtual ~IPC(){
        _param.clear();
    };   

    /// The getXML() method returns a string with the current parameter settings of the object in XML format.
    /**
    The XML string can be used for saving, loading and manipulating the parameters.
    */
    virtual std::string getXML()            
    {   stringstream xml;
        if (!_param.empty()){
            xml << "<" << _identifyerString << ">";
            for (std::map<std::string, DoubleParameter*>::iterator it = _param.begin(); it != _param.end() ; it++){
                      xml << (it->second)->getXML();
                  }
            xml << "</" << _identifyerString << ">";
        }
        return string(xml.str());
    };


    /// The setXML() method tries to interpet the XML string to set the parameters of this object
    /**
    The XML string can be used for saving, loading and manipulating the parameters.
    */
                
    virtual bool setXML(std::string xmlString){
                    TiXmlDocument doc;
                    doc.Parse(xmlString.c_str());
                    logDeb( "IPC received xml: " << doc );
                    
                    //create a safe handle
                	TiXmlHandle hDoc(&doc);
                	TiXmlElement* pElem;
                	TiXmlHandle hRoot(0);
                    pElem=hDoc.FirstChildElement(_identifyerString).Element();
            		// should always have a valid root but handle gracefully if it does not
            		if (!pElem) return false;
                
                    logDeb( _identifyerString << " Root " << pElem->Value() );
            		// save this as root handle
            		hRoot=TiXmlHandle(pElem);

                    //now iterate over all siblings
            		pElem=hRoot.FirstChild().Element();
            		if (!pElem) {cout << "error, false pointer"<< endl; return false;}
                    int level1Count = 0;
                    for( pElem=pElem; pElem; pElem=pElem->NextSiblingElement())
                    {
                        logDeb("sibling "<< level1Count++ << " "<< pElem->Value());
                        logDeb("raw: " << *pElem );
                        
                        // TiXmlElement* pElem3=pElem->FirstChild()->ToElement();
                        TiXmlElement* pAttribute=pElem->ToElement();
                        if (!pAttribute) {cout << "error, false pointer"<< endl; return 1;}
                        double value, min, max, step;
                        string name;
                        pAttribute->QueryStringAttribute ("name",&name);
                        pAttribute->QueryDoubleAttribute ("value",&value);
                        pAttribute->QueryDoubleAttribute ("min",&min);
                        pAttribute->QueryDoubleAttribute ("max",&max);
                        pAttribute->QueryDoubleAttribute ("step",&step);
                    
                        logDeb("Name:" << name <<", value:" << value << ", min:" << min << ", max:" << max << ", step:" << step);

                        _param[name] = new DoubleParameter(value,min,max,step,name);                        
                        

                    }
                    return true;
    }            
              
    /// The save() method saves the parameters of the container to a file.
    /**
    This method saves the settings of the container to a file, so that the container can be restored in an identcal state using the load() method.
    */
    virtual bool save(const std::string& name)
    {   
        logDeb("IPC Save called...");
        if (!_param.empty()){
            TiXmlDocument doc(name);
            doc.Parse(getXML().c_str());
            // doc.Print();
            doc.SaveFile();
        }
        return true;
    }

    /// The load() method loads the parameters of the container from a file.
    /**
    This method loads the settings of the container from a file that was saved by an identical type of container using the save() method.
    */
    virtual bool load(const std::string& name)
    {
        logDeb( "IPC Load called...");
        if (!_param.empty()){
            TiXmlDocument doc(name);
            doc.LoadFile();
            // doc.Print();
            stringstream xmlString;
            xmlString << doc;
            logDeb( "xmlString from load = " << xmlString.str());
            setXML(xmlString.str());
        }
        return true;
    }

    /// Set the input image of an image processing container.
    virtual void inputImage(cv::Mat inputImage){
            _inputImage = inputImage;
            return;
    }

    /// Return the output image of an image processing container.
    virtual cv::Mat outputImage(){
        return _outputImage;
    }

    /// Execute the main functionality of the image processing container. 
    /**
    For process() to work, we need a valid inputImage. As a result a valid outputImage is created.
    */
    virtual void process(){
        _outputImage = _inputImage;
        return;
    }
    
    void interactiveTune(int posX, int posY)
    {
        if (!_param.empty()){
        
            cv::namedWindow( _identifyerString, cv::WINDOW_AUTOSIZE );
            cv::moveWindow(_identifyerString, posX , posY );
      
      
            std::map<std::string, DoubleParameter*>::iterator it = _param.begin();
            std::map<std::string, DoubleParameter*>::iterator itEnd;
      
            for(;;){
             int c = (char)cv::waitKey( IPC_wait_key_period );
    
             if( c == 27 ){ break; } //esc
             if( c == 'n' )
             {
                 itEnd = _param.end();
                 if ((it != _param.end()) && (it != --itEnd)){
                     it++;
                 }
             }
             if( c == 'p' )
             {
                 if (it != _param.begin()){
                     it--;
                 }
             }
           
             if( c == '+' ){ it->second->incr(); }//
             if( c == '-' ){ it->second->decr(); }//
    
    
             cout << "\r" << _identifyerString << "[" << it->first << "] = " << it->second->get() << "            " <<flush;
       
             process();
       
             if (_outputImage.data)
                 cv::imshow(_identifyerString, _outputImage);
           }
           //cleanup
           cout << endl;
            cv::destroyWindow(_identifyerString);
           return;
          }
      }; 
      /// Get the identifying string from this object.
      string getIdentifierString(){return _identifyerString;};    
};


}//namespace ipc

#endif //__IMAGEPROCESSINGCONTAINER__HPP__
