cmake_minimum_required(VERSION 3.0.0)
project(SyncEventBuffer VERSION 0.1.0)

#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#the file(GLOB...) allows for wildcard additions:
file(GLOB INCLUDES "*.h")
file(GLOB SOURCES "*.cpp")


include_directories(${INCLUDES})
add_executable(SyncEventBuffer ${SOURCES})

#Link pthread
#target_link_libraries(SyncEventBuffer PUBLIC pthread)
set_target_properties(SyncEventBuffer PROPERTIES LINK_LIBRARIES -pthread)