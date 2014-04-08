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
    
    
    // Image_Processing_Type operation;
	string imageFileName;
	cv::Mat inputImage;
	cv::Mat pipelineImage(inputImage);
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

    //create the pipeline
    IPCPipeline* ipp = new IPCPipeline();
    ipp->inputImage(inputImage);
    ipp->process();
    pipelineImage = ipp->outputImage();

    //create an Image Processing Container, that connects to the output of the pipeline
    IPC* ipc = new IPC();
    ipc->inputImage(pipelineImage);
    ipc->process();
    outputImage = ipc->outputImage();


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

            /*special features*/
        if( c == 't' ){
            cout << "Tuning Mode! " << endl;
            ipc->interactiveTune(100,100);
            ipc->save(ipc->getIdentifierString() + ".dat");
            cout << "Done Tuning. Saved parameters as " << ipc->getIdentifierString() + ".dat" << endl;
            
        } 

            /*pipeline functionality*/
        if( c == 'i' ){  
            cout << "Adding container to pipeline..." << endl;
            IPC* newIPC = IPCGen::createIPC(ipc->getIdentifierString());
            newIPC->load(ipc->getIdentifierString() + ".dat");
            ipp->push_back(newIPC);
            pipelineImage = ipp->outputImage();
            delete ipc; 
            ipc = new IPC(); 
            ipc->inputImage(pipelineImage); 
            cout << " Idle operation selected!" << endl;
        } 
        if( c == 'e' ){  
            ipp->pop_back();
            pipelineImage = ipp->outputImage();
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
        if( c == 'w' ){  
            cout << "Pipeline contents:" << endl;
            cout << ipp->getXML() << endl;
        } 

  
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
        pipelineImage = ipp->outputImage();
//-----------------------------------------------------------------------------------------------------------------------------------------------------//
        /* process ipc */


            /*choose output*/
        if( c == '0' ){ delete ipc; ipc = new IPC(); ipc->inputImage(pipelineImage); cout << " Idle operation selected!" << endl;}//
        if( c == '1' ){ delete ipc; ipc = IPCGen::createIPC("GaussianBlurIsotropic"); ipc->load(ipc->getIdentifierString() + ".dat"); ipc->inputImage(pipelineImage); cout << " GaussianBlurIsotropic operation selected!" << endl;}//
        if( c == '2' ){ delete ipc; ipc = IPCGen::createIPC("Threshold"); ipc->load(ipc->getIdentifierString() + ".dat"); ipc->inputImage(pipelineImage); cout << " Threshold operation selected!" << endl;}//
        if( c == '3' ){ delete ipc; ipc = IPCGen::createIPC("Laplacian"); ipc->load(ipc->getIdentifierString() + ".dat"); ipc->inputImage(pipelineImage); cout << " Laplacian operation selected!" << endl;}//
        if( c == '4' ){ delete ipc; ipc = IPCGen::createIPC("MorphologyEx"); ipc->load(ipc->getIdentifierString() + ".dat");;ipc->inputImage(pipelineImage); cout << " MorphologyEx operation selected!" << endl;}//



        ipc->inputImage(pipelineImage);
        ipc->process();
        outputImage = ipc->outputImage();

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
    delete ipc;

	return 0;
};


	/**********************************************************/












