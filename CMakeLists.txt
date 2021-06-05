cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILE_FEATURES OFF)
set(CMAKE_CXX_COMPILER "g++-10")
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fno-implicit-templates")


project(DoppelkopfUI VERSION 0.5.0)

include(FetchContent)
include(CTest)
enable_testing()

add_library(doppelkopf SHARED src/doppelkopf/action.cpp src/doppelkopf/doppelkopf.cpp src/doppelkopf/gamemode.cpp src/doppelkopf/pattern.cpp src/doppelkopf/state.cpp src/doppelkopf/interface/dki_stream.cpp)
add_executable(DoppelkopfUI src/main.cpp)

include_directories(PUBLIC include)
target_link_libraries(DoppelkopfUI doppelkopf)

##########################################################################################
# Libraries
##########################################################################################

##########################################################################################
# DOXYGEN
##########################################################################################
option(BUILD_DOC "Build documentation" ON)
find_package(Doxygen)

#if (BUILD_DOC)
	if (DOXYGEN_FOUND)
		set(DOXYGEN_OUTPUT_DIRECTORY doc)
		set(DOXYGEN_QUIET YES)
		#set(DOXYGEN_EXTRACT_PRIVATE YES)
		set(DOXYGEN_STRIP_FROM_PATH ./include/doppelkopf)
		set(DOXYGEN_USE_MDFILE_AS_MAINPAGE readme.md)
		set(DOXYGEN_GENERATE_TREEVIEW YES)
		
		doxygen_add_docs(doxygen readme.md ${PROJECT_SOURCE_DIR}/include/doppelkopf/ docs/ ALL COMMENT "Generate documentation")
	else(DOXYGEN_FOUND)
		message("No doxygen installation was found")
	endif(DOXYGEN_FOUND)
#endif()

##########################################################################################
# Testing
##########################################################################################
# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
	add_subdirectory(tests)
endif()

##########################################################################################
# Code Coverage
##########################################################################################
