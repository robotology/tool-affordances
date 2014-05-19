cmake_minimum_required(VERSION 2.6)

set(PROJECTNAME exploreAffordances)

project(${PROJECTNAME})

find_package(YARP)
find_package(OpenCV)
find_package(ICUB)

include_directories(${ICUB_INCLUDE_DIRS}
                    ${YARP_INCLUDE_DIRS}
					${OpenCV_INCLUDE_DIRS} )
                    
add_executable(askForTool askForTool.cpp)
target_link_libraries(askForTool ${YARP_LIBRARIES} icubmod)
