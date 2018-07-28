cmake_minimum_required(VERSION 3.0)
project(rotImgPj)

set(NAME_SRC 
	src/main.c
	src/bmpIO.c
	src/imgFormat.c
	src/lookupTable.c
	src/rotLib.c	
)

set(NAME_HEADERS
	include/bmpIO.h
	include/imgFormat.h
	include/lookupTable.h
	include/rotImgLib.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_executable(rotImgPj ${NAME_SRC} ${NAME_HEADERS})
target_link_libraries(rotImgPj ${OpenCV_LIBS})