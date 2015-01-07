include (GenerateExportHeader)

# Setup project environment
GET_FILENAME_COMPONENT(BASE_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
SET(WORKSPACE_DIR ${BASE_DIR}/..)
SET(SOURCE_DIR ${WORKSPACE_DIR}/src)
      

#setup tinyxml
set(TINYXMLDIR ${WORKSPACE_DIR}/tinyxml)
SET( LIB_NAME tinyxml)  
SET( TINYXML_LIBS "${LIB_NAME}")
PROJECT( ${LIB_NAME} )  
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)  
add_definitions( -DTIXML_USE_STL )
INCLUDE_DIRECTORIES(TINYXMLDIR)
add_library(${LIB_NAME} STATIC
        ${TINYXMLDIR}/tinyxml.cpp
        ${TINYXMLDIR}/tinyxmlparser.cpp
        ${TINYXMLDIR}/tinystr.cpp
        ${TINYXMLDIR}/tinyxmlerror.cpp
      )
      
#setup executable
SET( PROJECT_NAME createIPCPipeline)  
PROJECT( ${PROJECT_NAME} )  
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)  
FIND_PACKAGE( OpenCV REQUIRED )  
message("OpenCV: ${OpenCV_INCLUDE_DIRS}")
INCLUDE_DIRECTORIES(${TINYXMLDIR} ${OpenCV_INCLUDE_DIRS})

ADD_EXECUTABLE( ${PROJECT_NAME} ${SOURCE_DIR}/createIPCPipeline.cpp
                ${SOURCE_DIR}/ipc_pipeline.cpp
      )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} 
				${OpenCV_LIBS} 
        ${TINYXML_LIBS}
) 




