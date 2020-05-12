cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(ptpmonkey LANGUAGES CXX)


add_library(ptpmonkey SHARED 	"source/loghander.cpp" 
				"source/mac.cpp" 
				"source/ptpclock.cpp" 
				"source/ptpeventloghandler.cpp" 
				"source/ptploghandler.cpp" 
				"source/ptpmonkey.cpp" 
				"source/ptpmonkeyhandler.cpp" 
				"source/ptpmonkeyimplementation.cpp" 
				"source/ptpparser.cpp" 
				"source/ptpstructs.cpp" 
                "source/receiver.cpp" 
				"source/sender.cpp" 
				"source/timeutils.cpp" )


target_include_directories(ptpmonkey PUBLIC ${PROJECT_SOURCE_DIR}/include)

#pml_log
find_path(LOG_INCLUDE_DIR NAMES "log.h" PATHS ${CMAKE_CURRENT_SOURCE_DIR}/external/log/include)
message(${CMAKE_CURRENT_SOURCE_DIR}/external/log/include)

if((NOT LOG_INCLUDE_DIR) OR (NOT EXISTS ${LOG_INCLUDE_DIR}))
    # we couldn't find the header files for FOO or they don't exist
    message("Unable to find pml::log")
    
    # we have a submodule setup for log, assume it is under external/log
    # now we need to clone this submodule
    execute_process(COMMAND git submodule update --init -- external/log
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

    add_subdirectory(${PROJECT_SOURCE_DIR}/external/log ${CMAKE_CURRENT_BINARY_DIR}/log)
    # set LOG_INCLUDE_DIR properly
    set(LOG_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/log/include
        CACHE PATH "log include directory")

    # also install it
    #set(asio_dest /usr/local/include)
    #install(DIRECTORY ${ASIO_INCLUDE_DIR}/asio DESTINATION ${asio_dest})

    # for convenience setup a target
    #add_library(asio INTERFACE)
    #target_include_directories(asio INTERFACE
    #                           $<BUILD_INTERFACE:${ASIO_INCLUDE_DIR}>
    #                           $<INSTALL_INTERFACE:${asio_dest}>)

    # need to export target as well
    #install(TARGETS asio EXPORT my_export_set DESTINATION ${asio_dest})
else()
	 add_subdirectory(${PROJECT_SOURCE_DIR}/external/log ${CMAKE_CURRENT_BINARY_DIR}/log)
	message("Found pml::log")
endif()

target_include_directories(ptpmonkey PUBLIC ${LOG_INCLUDE_DIR})

# ASIO
find_path(ASIO_INCLUDE_DIR NAMES "asio.hpp" PATHS ${CMAKE_CURRENT_SOURCE_DIR}/external/asio/asio/include)
message(${CMAKE_CURRENT_SOURCE_DIR}/external/asio/asio/include)

if((NOT ASIO_INCLUDE_DIR) OR (NOT EXISTS ${ASIO_INCLUDE_DIR}))
    # we couldn't find the header files for FOO or they don't exist
    message("Unable to find Asio")
    

    # we have a submodule setup for asio, assume it is under external/asio
    # now we need to clone this submodule
    execute_process(COMMAND git submodule update --init -- external/asio
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

    # set ASIO_INCLUDE_DIR properly
    set(ASIO_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/asio/asio/include
        CACHE PATH "asio include directory")

    # also install it
    #set(asio_dest /usr/local/include)
    #install(DIRECTORY ${ASIO_INCLUDE_DIR}/asio DESTINATION ${asio_dest})

    # for convenience setup a target
    #add_library(asio INTERFACE)
    #target_include_directories(asio INTERFACE
    #                           $<BUILD_INTERFACE:${ASIO_INCLUDE_DIR}>
    #                           $<INSTALL_INTERFACE:${asio_dest}>)

    # need to export target as well
    #install(TARGETS asio EXPORT my_export_set DESTINATION ${asio_dest})
else()
	message("Found Asio")
endif()


target_include_directories(ptpmonkey PUBLIC ${ASIO_INCLUDE_DIR})

list(APPEND flags "-Wall" "-s" "-O3" "-fexceptions")
target_compile_options(ptpmonkey PRIVATE ${flags})
target_compile_definitions(ptpmonkey PUBLIC ASIO_STANDALONE)

target_link_libraries(ptpmonkey pml_log)

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_compile_definitions(ptpmonkey PRIVATE __GNU__)
	target_link_libraries(ptpmonkey pthread)
endif()

set_target_properties(ptpmonkey PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../lib/")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS ptpmonkey LIBRARY DESTINATION /usr/local/lib)
endif()


project(ptpmonkeyexample LANGUAGES CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("include")
link_directories("lib")
add_executable(ptpmonkeyexample examples/main.cpp)
list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3" "-std=c++14")
target_compile_options(ptpmonkeyexample PRIVATE ${flags})
target_compile_definitions(ptpmonkeyexample PUBLIC NDEBUG DLL_EXPORTS)

target_link_libraries(ptpmonkeyexample ptpmonkey)

set_target_properties(ptpmonkeyexample PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../bin")


