//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//general libraries
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>     /* atoi */

#include <time.h>
#include <string>
#include <sstream>

#include <cv.h>

#include "log.h"
#include "ipc.h"
#include "ipc_pipeline.h"


using namespace std;
using namespace ipc;
using namespace cv;

enum SourceMode
{
    SM_FILE,
    SM_CAMERA,
    SM_IDLE,
    SM_COUNT    
}sourceMode;

inline bool readImageFromDisk(cv::Mat& inputImage, string imageFileName){

    if(imageFileName.empty())
        return false;

    inputImage = cv::imread(imageFileName,CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);

    if( !inputImage.data )
        return false;
    
    cout <<"read " << imageFileName << "from disk..." << endl;
    return true;
}

inline bool openVidoCamera(cv::VideoCapture& cap){

    cap =  cv::VideoCapture(0);

    if(!cap.isOpened())
        return false;
    
    return true;
}

int main(int argc, char* argv[])
{
	//basic variables
	int rows, cols;
    Image_Processing_Type operation;
	string imageFileName;
	cv::Mat inputImage;
	cv::Mat processedImage(inputImage);
	cv::Mat outputImage(inputImage);
    cv::VideoCapture cap;
 
	//check arguments
	if (argc > 2){
		printf("usage: %s [image_file] \n", argv[0]);
		return 0;
	} else if (argc == 2){
    	imageFileName = argv[1];
	}
    
    if (readImageFromDisk(inputImage, imageFileName)){
        sourceMode = SM_FILE;
        cout << "file mode " << endl;
    } else if (openVidoCamera(cap)){  
        cap >> inputImage;
        sourceMode = SM_CAMERA;
        cout << "camera mode " << endl;
    } else {
        sourceMode = SM_IDLE;
        inputImage = cv::Mat(100, 100, CV_8UC3);
    }
    outputImage = inputImage.clone();

    vector<IPC*> displayOperation(IP_COUNT);
    
    for (Image_Processing_Type i = IP_NONE; i < IP_COUNT ; i++){
        displayOperation[i] = IPCGen::createIPC(i);
    }

    //for all operations do:
    for (vector<IPC*>::iterator it = displayOperation.begin(); it != displayOperation.end() ; it++){
        if(*it != NULL){
            (*it)->load((*it)->getIdentifierString() + ".dat");
            cout << (*it)->getIdentifierString() + ".dat" << ":\n" << (*it)->getXML() << endl;
        }
    }

    //create the pipeline
    IPCPipeline* ipp = new IPCPipeline();
    ipp->inputImage(inputImage);
    ipp->process();
    processedImage = ipp->outputImage();

    //create windows
    imshow("inputImage", inputImage);
    imshow("outputImage", outputImage);

    //position windows
    cvMoveWindow( "inputImage", 0 , 0 );
    cvMoveWindow( "outputImage", 1000 , 0 );
    
    
    
//-----------------------------------------------------------------------------------------------------------------------------------------------------//
    //the main loop
    for(;;)
      {
        char c;
        c = (char)cv::waitKey( 1 );
      
        if( c == 'q' ){ cout << "quitting!" << endl; break; } //quit

            /*choose input*/
        //if the dimension change the application crashes
        // if( c == 'f' ){ cap.release(); if (readImageFromDisk(inputImage, imageFileName)){sourceMode = SM_FILE;} else {sourceMode = SM_IDLE;cout << "idle sourceMode " << endl;}}// use image file from disk
        // if( c == 'c' ){ if (openVidoCamera(cap)){sourceMode = SM_CAMERA;cout << "camera mode " << endl;} else {sourceMode = SM_IDLE;cout << "idle mode " << endl;}}// use webcam
        // if( c == 'i' ){  cap.release(); sourceMode = SM_IDLE; cout << "idle mode " << endl;}//

            /*special features*/
        if( c == 't' ){
            if( operation == IP_NONE){cout << "No output filter selected! " << endl;}
            else{
                cout << "Tuning Mode! " << endl;
                displayOperation[operation]->interactiveTune(100,100);
                displayOperation[operation]->save(displayOperation[operation]->getIdentifierString() + ".dat");
                cout << "Done Tuning. Saved parameters as " << displayOperation[operation]->getIdentifierString() + ".dat" << endl;
            }
        } 

            /*pipeline functionality*/
        if( c == 'i' ){  
            if( operation == IP_NONE){cout << "No output filter selected! " << endl;}
            else{
                cout << "Adding container to pipeline..." << endl;
                IPC* pipelineCandidate = IPCGen::createIPC(operation);
                pipelineCandidate->load(displayOperation[operation]->getIdentifierString() + ".dat");
                ipp->push_back(pipelineCandidate);
                processedImage = ipp->outputImage();
                operation = IP_NONE;
            }
        } 
        if( c == 'e' ){  
            ipp->pop_back();
            processedImage = ipp->outputImage();
        } 
        if( c == 's' ){  
             cout << "Save pipeline as:" << flush;
             string pipelinename;
             cin >> pipelinename;
            ipp->save(pipelinename);
        } 
        if( c == 'l' ){  
            cout << "Load pipeline:" << flush;
            string pipelinename;
            cin >> pipelinename;
            ipp->load(pipelinename);
        } 



            /*choose output*/
        if( c == '0' ){ operation = IP_NONE; cout << " Idle operation selected!" << endl;}//
        
        if( atoi(&c) > 0 && atoi(&c) < IP_COUNT )
            { operation = (Image_Processing_Type)atoi(&c); cout << " Operation " << displayOperation[operation]->getIdentifierString() << " selected!" << endl; }//

//-----------------------------------------------------------------------------------------------------------------------------------------------------//
        /* process input image */
        if (sourceMode == SM_FILE){
        } else 
        if (sourceMode == SM_CAMERA){
            cap >> inputImage; //refresh webcam video
        } 
//-----------------------------------------------------------------------------------------------------------------------------------------------------//
        /* process pipline */
        ipp->process();
        processedImage = ipp->outputImage();
//-----------------------------------------------------------------------------------------------------------------------------------------------------//
        /* process output  image */
        if( operation == IP_NONE){
            outputImage = processedImage;          
        }
        else{
            displayOperation[operation]->inputImage(processedImage);
            displayOperation[operation]->process();
            outputImage = displayOperation[operation]->outputImage();          
            
        }       
 //-----------------------------------------------------------------------------------------------------------------------------------------------------//
        
        /* display windows */
        if (inputImage.data)
            imshow("inputImage", inputImage);
        if (outputImage.data)
            imshow("outputImage", outputImage);

      } // end of for loop
//-----------------------------------------------------------------------------------------------------------------------------------------------------//
      //cleanup
    cout << endl;
    cap.release();
    cvDestroyWindow("inputImage");
    cvDestroyWindow("outputImage");
    delete ipp;
    for (vector<IPC*>::iterator it = displayOperation.begin(); it != displayOperation.end() ; it++){
        if(*it != NULL){
            delete *it;
            *it = NULL;
        }
    }
    
	return 0;
};


	/**********************************************************/












