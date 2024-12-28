cmake_minimum_required(VERSION 3.10)

project(TinyXML)
message(STATUS "Prepare: ${PROJECT_NAME}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-DTIXML_USE_STL)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_WIN32_WINNT=0x0501)

include_directories(
)

set(SOURCE_FILES
	tinyxml.cpp
	tinystr.cpp
	tinyxmlerror.cpp
	tinyxmlparser.cpp
)

set(HEADER_FILES
	tinystr.h
	tinyxml.h
)

add_library(${PROJECT_NAME} STATIC
	${SOURCE_FILES} ${HEADER_FILES}
	)

target_include_directories(${PROJECT_NAME}
	PUBLIC .
)
