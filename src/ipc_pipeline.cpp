//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>     /* atoi */

#include <time.h>
#include <string>
#include <sstream>

#include <opencv2/opencv.hpp>

#include "log.h"
#include "ipc_pipeline.h"
#include "ipc.h"

using namespace std;
namespace ipc {

    
/*------------------------------------------------------------------------ 
   Pipeline   
*------------------------------------------------------------------------*/

void IPCPipeline::push_back(IPC* ipc){
    logDeb("IPCPipeline push_back() called"); 
    _container.push_back(ipc);                      //now add this ipc to the pipeline
    _container.back()->inputImage(_result.back());  //last result is input for next ipc
    _container.back()->process();                   //we have to process once for the output to become available
    _result.push_back(_container.back()->outputImage());  //register the output image
    logDeb( "output of pipeline set to " << _container.back()->getIdentifierString());
}

void IPCPipeline::pop_back(){
    logDeb("IPCPipeline pop_back() called"); 
    
    if (_container.size()>0){
        logDeb("removeing last container from pipeline...");
        
        delete _container.back();
        _container.pop_back();
        _result.pop_back();
        
    }
     
}

IPC* IPCPipeline::back(){
    if (_container.size()>0){
        return _container.back();
    } else {
        return this;
    }
    
     
}

std::string IPCPipeline::getXML()            
{
    stringstream xml;
    xml << "<" << _identifyerString << ">";
    for (vector<IPC*>::iterator it = _container.begin(); it != _container.end() ; it++){
              if(*it != NULL){
                  xml << (*it)->getXML();
              }
          }
    xml << "</" << _identifyerString << ">";
    return string(xml.str());
};

bool IPCPipeline::setXML(std::string xmlString){
    
                reset(); //empty the current pipeline
    
                TiXmlDocument doc;
                doc.Parse(xmlString.c_str());
                logDeb( "Pipeline received xml: " << doc );
                
                //create a safe handle
            	TiXmlHandle hDoc(&doc);
            	TiXmlElement* pElem;
            	TiXmlHandle hRoot(0);
                pElem=hDoc.FirstChildElement(_identifyerString).Element();
        		// should always have a valid root but handle gracefully if it does not
        		if (!pElem) return false;
                
                logDeb( "Pipeline Root " << pElem->Value());
        		// save this as root handle
        		hRoot=TiXmlHandle(pElem);

                //now iterate over all siblings
        		pElem=hRoot.FirstChild().Element();
        		if (!pElem) {cout << "error, false pointer"<< endl; return false;}
                int level1Count = 0;
                for( pElem=pElem; pElem; pElem=pElem->NextSiblingElement())
                {
                    logDeb("sibling "<< level1Count++ << " "<< pElem->Value() );
                    logDeb("raw: " << *pElem );
                    
                    //create a new IPC of the type found
                    IPC* newIPC =  IPCGen::createIPC(pElem->Value());
                    if (newIPC != NULL){ // check if this object was created before using it
                        stringstream ipcString;
                        ipcString << *pElem;
                        newIPC->setXML(ipcString.str().c_str());
                        //insert this object into the pipeline
                        push_back(newIPC);
                    }    
                }
                return true;
}         

void IPCPipeline::process(){
    logDeb("IPCPipeline process() called, container size:" << _container.size()); 
    
    //initiate all the process functions of the containers in theis pipeline
    for (unsigned i=0; i<_container.size(); i++){
        _container.at(i)->inputImage(_result.at(i));
        _container.at(i)->process();
        _result.at(i+1) = _container.at(i)->outputImage();
    }    

    return;
};

// void IPCPipeline::interactiveTune(int posX, int posY){
//     cv::namedWindow( _identifyerString, cv::WINDOW_AUTOSIZE );
//     cv::moveWindow(_identifyerString, posX , posY );
// 
//     for(;;)
//     {
//         int c = (char)cv::waitKey( IPC_wait_key_period );
// 
//         if( c == 27 ){ break; }
//         if( c == 'd' ){
//             cout << "Dir name:" << flush;
//             cin >> _dirname;
//         }
//         if( c == 'f' ){
//             cout << "File name:" << flush;
//             cin >> _filename;
//         }
//         if( c == 'l' ){
//             load(_filename);
//         }
//         if( c == 's' ){
//             save(_filename);
//         }
//     
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






}//namespace ipc
