
cmake_minimum_required(VERSION 3.10)

project(lft_tsp)
message(STATUS "Prepare: ${PROJECT_NAME}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(HEADER_FILES
	lft_tsp.h
)

set(SOURCE_FILES
	main.cpp
	lft_tsp.cpp
)

if(WIN32)
	set(WINDOWS_RESOURCE_FILES
		${PROJECT_NAME}.rc
		mipt.ico
	)
endif(WIN32)

set(ICON_RESOURCE_FILES
	mipt.ico
)

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${HEADER_FILES})
source_group("src.gen" FILES ${SRCGEN_RCC_FILES} ${QM_FILES} ${SRCGEN_QRC_QM_FILES})
source_group("Resource" FILES ${ICON_RESOURCE_FILES} ${WINDOWS_RESOURCE_FILES})

add_executable(${PROJECT_NAME}
	${HEADER_FILES}
	${SOURCE_FILES}
	${ICON_RESOURCE_FILES}
	${WINDOWS_RESOURCE_FILES}
)

include_directories(
	../lf_common
	../lf_assignment
	../../3rd_party
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
	Qt6::Gui
	lf_common
	lf_assignment
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
