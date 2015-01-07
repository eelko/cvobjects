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

#include <opencv2/opencv.hpp>

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
    
    cout <<"read " << imageFileName << " from disk..." << endl;
    return true;
}

inline bool openVidoCamera(cv::VideoCapture& cap){

    cap =  cv::VideoCapture(0);

    if(!cap.isOpened())
        return false;
    
    return true;
}

inline void changeImageContainer(IPC* &ipc, string identifier){
    delete ipc; 
    ipc = IPCGen::createIPC(identifier.c_str()); 
    ipc->load(ipc->getIdentifierString() + ".dat"); 
    cout <<  "\r" << identifier << " operation selected!                                                      " << flush;
}

string getHeader(){
    stringstream header;
    header << "Menu:" << endl; 
    header << "m \t- Print this Menu!" << endl;
    header << "q \t- Quit!" << endl; 
    header << "0 \t- Idle IPC" << endl; 
    header << "1 \t- Blur" << endl; 
    header << "2 \t- Threshold" << endl; 
    header << "3 \t- Laplace" << endl; 
    header << "4 \t- Morphological Operation" << endl; 
    header << "5 \t- Color Segmentation HSV" << endl; 
	header << "-----------------------------" << endl; 
	header << "t \t- Tune specific settings of the selected filter" << endl; 
	header << "i \t- Insert current operation into pipeline" << endl; 
    header << "e \t- Eject current operation from pipeline" << endl; 
    header << "s \t- Save current pipeline (ctrl-tab to commandline to enter name)" << endl; 
    header << "l \t- Load pipeline from file (ctrl-tab to commandline to enter name)" << endl; 
    header << "w \t- Print current pipeline XML string" << endl; 
    return header.str();
    
}
int main(int argc, char* argv[])
{
	//basic variables
	int rows, cols;
    
    
    // Image_Processing_Type operation;
	string imageFileName; /// if we do not use the camera we have an imput image whith this name
	string referenceFileName; /// if we want we can gif this image as a reference for image processing
	cv::Mat inputImage;
	cv::Mat referenceImage;
	cv::Mat pipelineImage(inputImage);
	cv::Mat outputImage(inputImage);
    cv::VideoCapture cap;
 
	//read arguments
    if ( (argc <= 1) || (argv[argc-1] == NULL) ) {  // there is NO input...
//			cout << "no arguments provided, argc = " << argc << " argv[argc-1]= " << argv[argc-1] <<  endl;
    }
	else{
    //some for statement that loops through the arguments
		for(int i = 1; i < argc; i++){
	//		cout << "argc = " << argc << " i=" << i << endl;
			string input = argv[i];
			if (input == "-i" ){
				if (i+1 < argc){
					imageFileName = argv[i+1];
					cout << "imagefilename: " << imageFileName << endl;
				}
			}
			else if (input == "-r" ){
				if (i+1 < argc){
					referenceFileName = argv[i+1];
					cout << "referencefilename: " << referenceFileName << endl;
					if (!readImageFromDisk(referenceImage,referenceFileName))
						cout << "could not read reference image!" << endl; 
				}
			}
			else if (input == "-h" || input == "--help"){
				cout << "usage: " << argv[0] << " [-i inputImage] [-r referenceImage]" << endl;
				return 0;
			}
		}
	}


	//detect mode of operation
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
	ipp->referenceImage(referenceImage);
    ipp->process();
    pipelineImage = ipp->outputImage();

    //create an Image Processing Container, that connects to the output of the pipeline
    IPC* ipc = new IPC();
    ipc->inputImage(pipelineImage);
	ipc->referenceImage(referenceImage);
	ipc->process();
    outputImage = ipc->outputImage();


    //create windows
    imshow("inputImage", inputImage);
    imshow("outputImage", outputImage);

    //position windows
    cvMoveWindow( "inputImage", 0 , 0 );
    cvMoveWindow( "outputImage", 1000 , 0 );
    
    cout << getHeader() << endl;
    
//-----------------------------------------------------------------------------------------------------------------------------------------------------//
    //the main loop
    for(;;)
      {
        char c;
        c = (char)cv::waitKey( 1 );
      
        if( c == 'q' ){ cout << "quitting!" << endl; break; } //quit
        if( c == 'm' ){ cout << getHeader() << endl;} 

            /*special features*/
        if( c == 't' ){
            cout << "Tuning Mode: ";
			cout << "use \'p\' and \'n\' to select operation and \'+\' and \'-\'to adjust the value" << endl;
            ipc->interactiveTune(100,100);
            ipc->save(ipc->getIdentifierString() + ".dat");
            cout << "Done Tuning. Saved parameters as " << ipc->getIdentifierString() + ".dat" << endl;
            
        } 

            /*pipeline functionality*/
        if( c == 'i' ){  
            IPC* newIPC = IPCGen::createIPC(ipc->getIdentifierString());
            newIPC->load(ipc->getIdentifierString() + ".dat");
            ipp->push_back(newIPC);
            pipelineImage = ipp->outputImage();
            cout << "\rAdded " << ipc->getIdentifierString() << " operation to pipeline...                          " << endl;
            changeImageContainer(ipc,"idle");
        } 
        if( c == 'e' ){  
            cout << "\rRemoved " << ipp->back()->getIdentifierString() << " operation from pipeline...                         " << endl;
            ipp->pop_back();
            pipelineImage = ipp->outputImage();
        } 
        if( c == 's' ){  
             cout << "\n Save pipeline as:                                                                      " << flush;
             string pipelinename;
             cin >> pipelinename;
			 cout << "saving pipeline as: "<< pipelinename << endl;
             ipp->save(pipelinename);
        } 
        if( c == 'l' ){  
            cout << "\nLoad pipeline:                                                                             " << flush;
            string pipelinename;
            cin >> pipelinename;
            ipp->load(pipelinename);
        } 
        if( c == 'w' ){  
            cout << "\n Pipeline contents:" << endl;
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
        /* process container */


            /*choose output*/
        if( c == '0' ){ changeImageContainer(ipc,"idle");}//
        if( c == '1' ){ changeImageContainer(ipc,"GaussianBlurIsotropic");}//
        if( c == '2' ){ changeImageContainer(ipc,"Threshold");}//
        if( c == '3' ){ changeImageContainer(ipc,"Laplacian");}//
        if( c == '4' ){ changeImageContainer(ipc,"MorphologyEx");}//
        if( c == '5' ){ changeImageContainer(ipc,"ColorSegmentHSV");}//
        if( c == '6' ){ changeImageContainer(ipc,"Registration");}//

        ipc->inputImage(pipelineImage);
		ipc->referenceImage(referenceImage);
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












