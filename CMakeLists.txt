
cmake_minimum_required(VERSION 2.8)


project(ValkyrieEngine)

function(VK_MOC prefix inlist outlist)
	set(out)
	foreach(file ${${inlist}})
		execute_process(COMMAND ClassGenerator "--file" ${prefix} ${CMAKE_CURRENT_BINARY_DIR} ${file} 
										WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
										RESULT_VARIABLE output)
		if(${output} EQUAL 1)
			string(REPLACE ".hh" ".refl.hh" refl_hh ${file})
			string(REPLACE ".hh" ".refl.cc" refl_cc ${file})
		
			set(out ${out} ${refl_hh} ${refl_cc})
		endif()
	endforeach(file)
	
	set(${outlist} ${out} PARENT_SCOPE)
endfunction(VK_MOC)

function(VK_INSTALL_MOCCER target prefix inlist)

	add_custom_command (TARGET ${target} PRE_BUILD 
										COMMAND ClassGenerator "--list" ${prefix} ${CMAKE_CURRENT_BINARY_DIR} ${${inlist}}
										WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
										)
endfunction(VK_INSTALL_MOCCER)


include_directories(${ValkyrieEngine_SOURCE_DIR}/ValkyrieEngine)
include_directories(${ValkyrieEngine_BINARY_DIR}/ValkyrieEngine)

add_subdirectory(ValkyrieEngine)
