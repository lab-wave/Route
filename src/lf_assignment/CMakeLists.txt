
cmake_minimum_required(VERSION 3.10)

project(lf_assignment)
message(STATUS "Prepare: ${PROJECT_NAME}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADER_FILES
	lf_assignment.h
)

set(SOURCE_FILES
	lf_assignment.cpp
)

if (CMAKE_VERSION VERSION_GREATER 3.7.8)
	if (MSVC_IDE)
		option(VS_ADD_NATIVE_VISUALIZERS "Configure project to use Visual Studio native visualizers" TRUE)
	else()
		set(VS_ADD_NATIVE_VISUALIZERS FALSE CACHE INTERNAL "Native visualizers are Visual Studio extension" FORCE)
	endif()

	# add natvis file to the library so it will automatically be loaded into Visual Studio
	if(VS_ADD_NATIVE_VISUALIZERS)
		set(NATVIS_FILES
			${PROJECT_NAME}.natvis
			)
	endif()
endif()

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${HEADER_FILES})
source_group("src.gen" FILES ${SRCGEN_RCC_FILES} ${QM_FILES} ${SRCGEN_QRC_QM_FILES})
source_group("Natvis" FILES ${NATVIS_FILES})

add_library(${PROJECT_NAME} STATIC
	${HEADER_FILES}
	${SOURCE_FILES}
	${NATVIS_FILES}
)

include_directories(
	../../3rd_party
	../lf_common
)

target_compile_definitions(${PROJECT_NAME} PRIVATE SIMULATOR_EXPORTS)

if(MSVC)
	target_compile_definitions(${PROJECT_NAME}	PRIVATE _CRT_SECURE_NO_WARNINGS)
endif(MSVC)

if(MSVC)
	set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_FLAGS_RELEASE /DEBUG)
endif(MSVC)

target_link_libraries(${PROJECT_NAME}
	Qt6::Core
	lf_common
)

if(${CMAKE_VERSION} VERSION_GREATER "3.8.0")
	set_target_properties(
		${PROJECT_NAME} PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()

#install(TARGETS
#	${PROJECT_NAME}
#	DESTINATION
#	.
#)
