
cmake_minimum_required(VERSION 3.10)

project(vlf)
message(STATUS "Prepare: ${PROJECT_NAME}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADER_FILES
	MassaObjem.hpp
	MyCSVDataStrReader.hpp
	MySyst.hpp
	MyTimerLogger.hpp
	TranspData.hpp
	VRPSolver.hpp
)

set(SOURCE_FILES
	main.cpp
	MassaObjem.cpp
	MyCSVDataStrReader.cpp
	MySyst.cpp
	MyTimerLogger.cpp
	TranspData.cpp
	VRPSolver.cpp
)

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${HEADER_FILES})

add_executable(${PROJECT_NAME}
	${HEADER_FILES}
	${SOURCE_FILES}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

include_directories(
)

if(MSVC)
	target_compile_definitions(${PROJECT_NAME}	PRIVATE _CRT_SECURE_NO_WARNINGS)
endif(MSVC)

if(MSVC)
	set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_FLAGS_RELEASE /DEBUG)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /UMBCS /D_UNICODE /DUNICODE")
endif(MSVC)
