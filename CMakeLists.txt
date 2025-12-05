cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include(addexternal.cmake)

project(ptpmonkey LANGUAGES CXX VERSION 1.2.0)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=ptpmonkey -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -DOUTPUT=${CMAKE_BINARY_DIR} -P ${CMAKE_CURRENT_SOURCE_DIR}/version.cmake)

SET(DIR_BASE ${CMAKE_SOURCE_DIR} CACHE STRING "base location for libraries")
SET(DIR_LOG ${DIR_BASE}/external/log CACHE STRING "location of pml log")
SET(DIR_ASIO ${DIR_BASE}/external/asio CACHE STRING "location of asio")

if(NOT DEFINED BUILD_LOG)
        SET(BUILD_LOG ON)
endif()


add_library(ptpmonkey SHARED 	"source/loghander.cpp"
				"source/mac.cpp"
				"source/management.cpp"
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
				"source/timeutils.cpp" ${CMAKE_BINARY_DIR}/src/ptpmonkey_version.cpp)



target_include_directories(ptpmonkey PUBLIC ${CMAKE_BINARY_DIR}/include)

#Get log
message(STATUS "Find pml::log")
add_external_library(pml_log ${DIR_LOG} "https://github.com/martim01/log.git" "")
add_external_library(pml_log ${DIR_ASIO} "https://github.com/chriskohlhoff/asio" "")

target_include_directories(ptpmonkey PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(ptpmonkey PUBLIC ${DIR_LOG}/include)
target_include_directories(ptpmonkey PUBLIC ${DIR_ASIO}/include)

SET(CMAKE_CXX_STANDARD 17)

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" )

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
else()
    target_compile_definitions(ptpmonkey PRIVATE ASIO_DISABLE_IOCP)
    target_link_libraries(ptpmonkey ws2_32 IPHLPAPI)
endif()

set_target_properties(ptpmonkey PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(ptpmonkey PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/)

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS ptpmonkey LIBRARY DESTINATION /usr/local/lib)
endif()

project(ptpmonkeyexample LANGUAGES CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("include")
include_directories(${DIR_ASIO})
link_directories("lib")
add_executable(ptpmonkeyexample examples/main.cpp)
list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3" "-std=c++17")
target_compile_options(ptpmonkeyexample PRIVATE ${flags})
target_compile_definitions(ptpmonkeyexample PUBLIC NDEBUG DLL_EXPORTS)


project(ptpmonkeysync LANGUAGES CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("include")
include_directories("sync/include")
include_directories(${DIR_ASIO})
link_directories("lib")
add_executable(ptpmonkeysync "sync/src/main.cpp" "sync/src/ptpsync.cpp" "sync/src/synceventhandler.cpp" "sync/src/inimanager.cpp" "sync/src/inisection.cpp")
list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3" "-std=c++17")
target_compile_options(ptpmonkeysync PRIVATE ${flags})
target_compile_definitions(ptpmonkeysync PUBLIC NDEBUG DLL_EXPORTS)


if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
else()
    target_compile_definitions(ptpmonkeyexample PRIVATE ASIO_DISABLE_IOCP)
    target_link_libraries(ptpmonkeyexample ws2_32 IPHLPAPI)
	target_compile_definitions(ptpmonkeysync PRIVATE ASIO_DISABLE_IOCP)
    target_link_libraries(ptpmonkeysync ws2_32 IPHLPAPI)
endif()

target_link_libraries(ptpmonkeyexample ptpmonkey)
target_link_libraries(ptpmonkeysync ptpmonkey)

set_target_properties(ptpmonkeyexample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set_target_properties(ptpmonkeysync PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)


