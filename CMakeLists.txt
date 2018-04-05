
cmake_minimum_required(VERSION 3.3)


project(CobaltSKY)


set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CobaltSKY_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CobaltSKY_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CobaltSKY_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CobaltSKY_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CobaltSKY_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CobaltSKY_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CobaltSKY_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CobaltSKY_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CobaltSKY_BINARY_DIR}/bin)



#function(CS_MOC prefix api inlist outlist)
#	set(out)
#	foreach(file ${${inlist}})
#		execute_process(COMMAND ClassGenerator "--file" ${prefix} ${api} ${CMAKE_CURRENT_BINARY_DIR} ${file} 
#										WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#										RESULT_VARIABLE output)
#		if(${output} EQUAL 1)
#			string(REPLACE ".hh" ".refl.hh" refl_hh ${file})
#			string(REPLACE ".hh" ".refl.cc" refl_cc ${file})
		
#			set(out ${out} ${refl_hh} ${refl_cc})
#		endif()
#	endforeach(file)
	
#	set(${outlist} ${out} PARENT_SCOPE)
#endfunction(CS_MOC)


function(CS_INSTALL_MOCCER target prefix api inlist)
	set(FILENAME "${CMAKE_CURRENT_BINARY_DIR}/.csmoc")
	message("MocFileName: ${FILENAME}")
	file(REMOVE "${FILENAME}")
	foreach(file ${${inlist}})
		file(APPEND "${FILENAME}" ${file} "\n")
	endforeach(file)
  if (MSVC_IDE)
		add_custom_command (TARGET ${target} PRE_BUILD
												COMMAND ${CobaltSKY_SOURCE_DIR}/bin/ClassGenerator ${prefix} ${api} ${CMAKE_CURRENT_BINARY_DIR} 
												WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
												)

	else()
		set(TARGET_NAME "${target}-MOC")
		add_custom_target(${TARGET_NAME}
											COMMAND ${CobaltSKY_SOURCE_DIR}/bin/ClassGenerator ${prefix} ${api} ${CMAKE_CURRENT_BINARY_DIR}
											WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
										)
		add_dependencies(${target} ${TARGET_NAME})
	endif()
endfunction(CS_INSTALL_MOCCER)

if (WIN32)
add_definitions(-DCS_WIN32)
else()
add_definitions(-DCS_LINUX)
endif()

find_package(OpenGL)
find_package(PNG)
find_package(Bullet)
find_package(GLEW)

include(${CobaltSKY_SOURCE_DIR}/Scripts/Findassimp.cmake)
#include(${CobaltSKY_SOURCE_DIR}/Scripts/FindGLEW.cmake)
include(${CobaltSKY_SOURCE_DIR}/Scripts/FindSDL2.cmake)
#include(${CobaltSKY_SOURCE_DIR}/Scripts/FindPhysX3.cmake)
#include(${CobaltSKY_SOURCE_DIR}/Scripts/FindBullet.cmake)

include_directories(${SDL_INCLUDE_DIR})
include_directories(${PNG_INCLUDE_DIRS})
include_directories(${BULLET_INCLUDE_DIRS})
include_directories(${CobaltSKY_SOURCE_DIR}/3rdparty)
include_directories(${CobaltSKY_SOURCE_DIR}/CobaltSKY)
include_directories(${CobaltSKY_BINARY_DIR}/CobaltSKY)


add_subdirectory(3rdparty)
add_subdirectory(CobaltSKY)
#add_subdirectory(Tools)
