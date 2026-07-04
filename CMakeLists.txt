cmake_minimum_required(VERSION 3.19)
project(bbb.noise)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/bbb_external.cmake)

set(C74_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/externals)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/bbb)

if(APPLE AND NOT CMAKE_OSX_ARCHITECTURES)
	set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "macOS architecture" FORCE)
endif()

option(BBB_NOISE_BUILD_TESTS "Build bbb.noise tests" OFF)

MACRO(SUBDIRLIST result curdir)
	FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
	SET(dirlist "")
	FOREACH(child ${children})
		IF(IS_DIRECTORY ${curdir}/${child})
			LIST(APPEND dirlist ${child})
		ENDIF()
	ENDFOREACH()
	SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(PROJECT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/source/projects)
foreach(project_dir ${PROJECT_DIRS})
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir}/CMakeLists.txt")
		message("Generating: ${project_dir}")
		add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir})
	endif()
endforeach()

if(BBB_NOISE_BUILD_TESTS)
	enable_testing()
	set(CMAKE_EXE_LINKER_FLAGS "")
	foreach(config DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
		set(CMAKE_EXE_LINKER_FLAGS_${config} "")
	endforeach()
	add_executable(bbb_noise_evaluator_tests tests/evaluator_tests.cpp)
	target_link_libraries(bbb_noise_evaluator_tests PRIVATE bbb_noise_eval)
	add_test(NAME bbb_noise_evaluator_tests COMMAND bbb_noise_evaluator_tests)
endif()

set(CMAKE_SUPPRESS_REGENERATION true)
