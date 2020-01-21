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
				"source/sender.cpp" 
				"source/timeutils.cpp" )


target_include_directories(ptpmonkey PUBLIC ${PROJECT_SOURCE_DIR}/include)

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
