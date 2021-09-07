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
target_include_directories(doppelkopf PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

add_executable(DoppelkopfUI src/main.cpp src/interfaces/websocket/wsmanager.cpp src/interfaces/websocket/wsobserver.cpp src/interfaces/websocket/wsobservermanager.cpp src/interfaces/websocket/wsplayer.cpp)

target_link_libraries(DoppelkopfUI PRIVATE doppelkopf)

##########################################################################################
# Libraries
##########################################################################################

#Needs boost: sudo apt-get install libboost-all-dev
#Needs OpenSSL: sudo apt-get install libssl-dev
FetchContent_Declare(
	simple_websocket_server_lib
	GIT_REPOSITORY https://gitlab.com/eidheim/Simple-WebSocket-Server.git
	GIT_TAG v2.0.2
)
FetchContent_MakeAvailable(simple_websocket_server_lib)
FetchContent_GetProperties(simple_websocket_server_lib)
target_include_directories(simple-websocket-server INTERFACE ${simple_websocket_server_lib_SOURCE_DIR}/>)
target_link_libraries(DoppelkopfUI PRIVATE simple-websocket-server)

find_package(nlohmann_json 3.2.0 REQUIRED) #https://github.com/nlohmann/json
target_link_libraries(doppelkopf PRIVATE nlohmann_json::nlohmann_json)

##########################################################################################
# DOXYGEN
##########################################################################################
option(BUILD_DOC "Build documentation" ON)
find_package(Doxygen)

if (BUILD_DOC)
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
endif()
unset(BUILD_DOC CACHE)

##########################################################################################
# LINTER
##########################################################################################
option(RUN_LINT "Run the linter" OFF)
if(RUN_LINT)
	find_program(CLANG_TIDY_COMMAND NAMES clang-tidy)
	if(NOT CLANG_TIDY_COMMAND)
		message(WARNING "Could not find clang-tidy!")
		set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE)
	else()
		#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
		set(CMAKE_CXX_CLANG_TIDY clang-tidy; -header-filter=.;)# -warnings-as-errors=*;)
	endif()
else()
	set(CMAKE_CXX_CLANG_TIDY "")
endif()
unset(RUN_LINT CACHE)

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
