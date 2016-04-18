
cmake_minimum_required(VERSION 3.3)


project(ValkyrieEngine)


set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ValkyrieEngine_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ValkyrieEngine_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ValkyrieEngine_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${ValkyrieEngine_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${ValkyrieEngine_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${ValkyrieEngine_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ValkyrieEngine_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${ValkyrieEngine_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${ValkyrieEngine_BINARY_DIR}/bin)


#function(VK_MOC prefix api inlist outlist)
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
#endfunction(VK_MOC)

function(VK_INSTALL_MOCCER target prefix api inlist)
    if (MSVC_IDE)
	add_custom_command (TARGET ${target} PRE_BUILD
										COMMAND ClassGenerator ${prefix} ${api} ${CMAKE_CURRENT_BINARY_DIR} ${${inlist}}
										WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
										)

    else()
	set(TARGET_NAME "${target}-MOC")
	add_custom_target(
		    ${TARGET_NAME}
										COMMAND ClassGenerator ${prefix} ${api} ${CMAKE_CURRENT_BINARY_DIR} ${${inlist}}
										WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
										)
	add_dependencies(${target} ${TARGET_NAME})
    endif()
endfunction(VK_INSTALL_MOCCER)

if (WIN32)
add_definitions(-DVK_WIN32)
else()
add_definitions(-DVK_LINUX)
endif()

find_package(OpenGL)
find_package(PNG)

include(${ValkyrieEngine_SOURCE_DIR}/Scripts/FindGLEW.cmake)
include(${ValkyrieEngine_SOURCE_DIR}/Scripts/FindSDL2.cmake)
include(${ValkyrieEngine_SOURCE_DIR}/Scripts/FindPhysX3.cmake)
include(${ValkyrieEngine_SOURCE_DIR}/Scripts/FindBullet.cmake)

include_directories(${SDL_INCLUDE_DIR})
include_directories(${PNG_INCLUDE_DIRS})
include_directories(${BULLET_INCLUDE_DIRS})
include_directories(${ValkyrieEngine_SOURCE_DIR}/3rdparty)
include_directories(${ValkyrieEngine_SOURCE_DIR}/ValkyrieEngine)
include_directories(${ValkyrieEngine_BINARY_DIR}/ValkyrieEngine)


add_subdirectory(3rdparty)
add_subdirectory(ValkyrieEngine)
add_subdirectory(Tools)
