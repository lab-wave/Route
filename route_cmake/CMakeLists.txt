
cmake_minimum_required(VERSION 3.10)

project(route)

message(STATUS "SYSTEM: ${CMAKE_SYSTEM_NAME}")

set(CMAKE_CONFIGURATION_TYPES Release Debug)
set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS ON)

set(SETUP_BIN_DIRECTORY "" CACHE PATH "")
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(SETUP_ENABLE_VRP_LF ON CACHE BOOL "")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

set(FIND_QT6_DIR "" CACHE PATH "Set qt6 dir here")
set(CMAKE_PREFIX_PATH ${FIND_OPENCV_DIR};${FIND_QT6_DIR})

message(STATUS "->Search qt6 libs")
find_package(Qt6 COMPONENTS Core Gui REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#------------------------------------------------------------------------------
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
	message(STATUS "->Move .editorconfig to:" ${CMAKE_CACHEFILE_DIR})
	configure_file(../src/.editorconfig ${CMAKE_CACHEFILE_DIR}/.editorconfig COPYONLY)

	if(MSVC)

		add_compile_definitions(WIN32)
		# return value
		set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /we4715 /we4716")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /we4715 /we4716")
		set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /we4715 /we4716")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /we4715 /we4716")

		# virtual/override
		set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /we4263 /we4264 /we4265")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /we4263 /we4264 /we4265")
		set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /we4263 /we4264 /we4265")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /we4263 /we4264 /we4265")
	else()

		message(FATAL_ERROR "Unsupported compiler!")

	endif()
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

	message(FATAL_ERROR "Unknown system!")

endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

#------------------------------------------------------------------------------


if(${SETUP_BIN_DIRECTORY} MATCHES "")
	get_filename_component(ABSOLUTE_BIN_DIRECTORY "${SETUP_BIN_DIRECTORY}" ABSOLUTE)
	
	message(STATUS "Set out directory to: ${ABSOLUTE_BIN_DIRECTORY}")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ABSOLUTE_BIN_DIRECTORY})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ABSOLUTE_BIN_DIRECTORY})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ABSOLUTE_BIN_DIRECTORY})
	set(CMAKE_PDB_OUTPUT_DIRECTORY ${ABSOLUTE_BIN_DIRECTORY})

	#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ABSOLUTE_BIN_DIRECTORY})
	#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${ABSOLUTE_BIN_DIRECTORY})
	#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${ABSOLUTE_BIN_DIRECTORY})
	#set(CMAKE_PDB_OUTPUT_DIRECTORY_RELEASE ${ABSOLUTE_BIN_DIRECTORY})
else(${SETUP_BIN_DIRECTORY})
	get_filename_component(ABSOLUTE_BIN_DIRECTORY "${SETUP_BIN_DIRECTORY}" ABSOLUTE)

	message(STATUS "Set out directory to default: ${ABSOLUTE_BIN_DIRECTORY}/bin")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ABSOLUTE_BIN_DIRECTORY}/lib)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ABSOLUTE_BIN_DIRECTORY}/bin)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ABSOLUTE_BIN_DIRECTORY}/bin)
	set(CMAKE_PDB_OUTPUT_DIRECTORY ${ABSOLUTE_BIN_DIRECTORY}/bin)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_CMake")
set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER "_CMake/autogen")
set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "src.gen")

add_subdirectory(../3rd_party/TinyXML/ ${ABSOLUTE_BIN_DIRECTORY}/TinyXML/)

set_property(
	TARGET
		TinyXML

	PROPERTY FOLDER "3rd_party/"
)

add_subdirectory(../src/lf_common ${ABSOLUTE_BIN_DIRECTORY}/lf_common)

set_property(
	TARGET
		lf_common

	PROPERTY FOLDER "lf/"
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
if(SETUP_ENABLE_VRP_LF)
	message(STATUS "VRP FRAMEWORK ENABLED")
	
	add_subdirectory(../src/vlf ${ABSOLUTE_BIN_DIRECTORY}/vlf)

	set_property(
		TARGET
			vlf

		PROPERTY FOLDER "vlf/"
	)
else(SETUP_ENABLE_VRP_LF)
	message(STATUS "VRP FRAMEWORK DISABLED")
endif(SETUP_ENABLE_VRP_LF)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")







