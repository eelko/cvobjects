//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef __IMAGEPROCESSINGCONTAINER_PIPELINE_2_HPP__
#define __IMAGEPROCESSINGCONTAINER_PIPELINE_2_HPP__

#include "ipc.hpp"

using namespace std;
namespace ipc {    
/*------------------------------------------------------------------------ 
   Pipeline   
*------------------------------------------------------------------------*/
    
class IPCPipeline : public IPC
{
private:
    string _dirname;
    string _filename;
    vector<IPC*> _container;
    vector<cv::Mat> _result;
    
public:
    IPCPipeline(string id = "IPCPipeline"){
        logDeb("IPCPipeline() constructor called"); 
        
        _identifyerString = id;
        _result.resize(1);
        _dirname = "pipeline_data";
        system((string("mkdir ./") + _dirname).c_str());
    };

    virtual ~IPCPipeline(){
        logDeb("IPCPipeline() desstructor called"); 
        for (vector<IPC*>::iterator it = _container.begin(); it != _container.end() ; it++){
            if(*it != NULL){
                delete *it;
                *it = NULL;
            }
        }
        _result.clear();
    };   


    virtual void reset(){ //Removes all elements from the vector (which are destroyed)
        logDeb("IPCPipeline reset() called"); 

        while (_container.size())
            pop_back();
    }
    
    
    //we redefine this inherited method because we use a vector
    virtual void inputImage(cv::Mat inputImage){
        logDeb("IPCPipeline inputImage() called"); 
        _result.at(0) = inputImage;
        return;
    }

    //we redefine this inherited method because we use a vector
    virtual cv::Mat outputImage(){
        logDeb("IPCPipeline outputImage() called"); 
        _outputImage = _result.back();
        return _outputImage;
    }
    
    virtual void push_back(IPC* ipc);
    virtual void pop_back();
    // virtual string parameterString();
    virtual void process();
    virtual std::string getXML();  
    virtual bool setXML(std::string xmlString);  

    // virtual void interactiveTune(int posX=0, int posY=0);
};




}

#endif //__IMAGEPROCESSINGCONTAINER_PIPELINE_2_HPP__
