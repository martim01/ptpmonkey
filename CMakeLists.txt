cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(ptpmonkey LANGUAGES CXX)

SET(DIR_BASE $ENV{HOME} CACHE STRING "base location for libraries")
SET(DIR_LOG ${DIR_BASE}/log CACHE STRING "location of pml log")
SET(DIR_ASIO ${DIR_BASE}/asio CACHE STRING "location of asio")

SET(BUILD_LOG ON CACHE BOOL "set to OFF if pml::log built by parent project")


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




#Get log
message(STATUS "Find pml::log")
find_path(PTP_TEMP_DIR NAMES "CMakeLists.txt" PATHS ${DIR_LOG})

if((NOT PTP_TEMP_DIR) OR (NOT EXISTS ${PTP_TEMP_DIR}))
        message(STATUS "log not found - clone from GitHub")
        execute_process(COMMAND git clone https://github.com/martim01/log/ ${DIR_LOG})

    find_path(PTP_TEMP_DIR2 NAMES "CMakeLists.txt" PATHS ${DIR_LOG})
    if((NOT PTP_TEMP_DIR2) OR (NOT EXISTS ${TEMP_DIR2}))
        message(FATAL_ERROR "Failed to clone pml::Log")
    endif()
else()
    message(STATUS "log found - update to latest version")
    execute_process(COMMAND git -C ${DIR_LOG} pull)
endif()


if(${BUILD_LOG})
	add_subdirectory(${DIR_LOG} ${DIR_LOG}/build)
else()
	message(STATUS "Restgoose - pml::log being build elsewhere")
endif()


# ASIO
find_path(PTP_TEMP_ASIO NAMES "asio.hpp" PATHS ${DIR_ASIO}/asio/include)
if((NOT PTP_TEMP_ASIO) OR (NOT EXISTS ${PTP_TEMP_ASIO}))
	message("asio not found - clone from GitHub")
	execute_process(COMMAND git clone https://github.com/chriskohlhoff/asio ${DIR_ASIO})

	find_path(PTP_TEMP_ASIO2 NAMES "asio.hpp" PATHS ${DIR_ASIO}/asio/include)
	if((NOT PTP_TEMP_ASIO2) OR (NOT EXISTS ${PTP_TEMP_ASIO2}))
        	message(FATAL_ERROR "Failed to clone asio")
	endif()
else()
	message(STATUS "asio found - update to latest version")
	execute_process(COMMAND git -C ${DIR_ASIO} pull)
endif()


target_include_directories(ptpmonkey PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(ptpmonkey PUBLIC ${DIR_LOG}/include)
target_include_directories(ptpmonkey PUBLIC ${DIR_ASIO}/asio/include)

list(APPEND flags "-fPIC" "-Wall" "-fpermissive"  "-std=c++14")

if(CMAKE_BUILD_TYPE MATCHES Release)
   list(APPEND flags "-O3")
   target_compile_definitions(ptpmonkey PUBLIC NDEBUG DLL_EXPORTS _MSL_STDINT_H)
else()
   list(APPEND flags "-g")
   target_compile_definitions(ptpmonkey PUBLIC DEBUG DLL_EXPORTS _MSL_STDINT_H)
endif()

target_compile_options(ptpmonkey PRIVATE ${flags})
target_compile_definitions(ptpmonkey PUBLIC ASIO_STANDALONE)

target_link_libraries(ptpmonkey optimized pml_log debug pml_logd)

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_compile_definitions(ptpmonkey PRIVATE __GNU__)
	target_link_libraries(ptpmonkey pthread)
endif()

set_target_properties(ptpmonkey PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(ptpmonkey PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/)

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS ptpmonkey LIBRARY DESTINATION /usr/local/lib)
endif()


#project(ptpmonkeyexample LANGUAGES CXX)
#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
#include_directories("include")
#link_directories("lib")
#add_executable(ptpmonkeyexample examples/main.cpp)
#list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3" "-std=c++14")
#target_compile_options(ptpmonkeyexample PRIVATE ${flags})
#target_compile_definitions(ptpmonkeyexample PUBLIC NDEBUG DLL_EXPORTS)

#target_link_libraries(ptpmonkeyexample ptpmonkey)

#set_target_properties(ptpmonkeyexample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)


