cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(ptpmonkey LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

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

file(GLOB SOURCES "source.cpp")

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(ptpmonkey PRIVATE ${flags})
target_compile_definitions(ptpmonkey PUBLIC NDEBUG DLL_EXPORTS CREATING_DLL _MSL_STDINT_H)


#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_compile_definitions(ptpmonkey PRIVATE __GNU__)
	target_link_libraries(ptpmonkey pthread)
endif()

set_target_properties(ptpmonkey PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../lib/")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS ptpmonkey LIBRARY DESTINATION /usr/local/lib)
endif()
