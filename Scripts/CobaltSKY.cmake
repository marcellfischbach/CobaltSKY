

function(CS_INSTALL_MOCCER target prefix api inlist)
	set(FILENAME "${CMAKE_CURRENT_BINARY_DIR}/.csmoc")
	message("MocFileName: ${FILENAME}")
	file(REMOVE "${FILENAME}")
	foreach(file ${${inlist}})
		file(APPEND "${FILENAME}" ${file} "\n")
	endforeach(file)
  if (MSVC_IDE)
		add_custom_command (TARGET ${target} PRE_BUILD
				COMMAND ${CobaltSKY_BINARY_DIR}/bin/ClassGenerator ${prefix} ${api} ${CMAKE_CURRENT_BINARY_DIR}
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
				)

	else()
		set(TARGET_NAME "${target}-MOC")
		add_custom_target(${TARGET_NAME}
				COMMAND ${CobaltSKY_BINARY_DIR}/bin/ClassGenerator ${prefix} ${api} ${CMAKE_CURRENT_BINARY_DIR}
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
				)
		add_dependencies(${target} ${TARGET_NAME})
		add_dependencies(${TARGET_NAME} ClassGenerator)
	endif()
endfunction(CS_INSTALL_MOCCER)




function(CS_MOC target prefix api inlist)
	set(FILENAME "${CMAKE_CURRENT_BINARY_DIR}/.csmoc")
	message("MocFileName: ${FILENAME}")
	file(REMOVE "${FILENAME}")
	foreach(file ${${inlist}})
		file(APPEND "${FILENAME}" ${file} "\n")
	endforeach(file)
	if (MSVC_IDE)
		add_custom_command (TARGET ${target} PRE_BUILD
				COMMAND ${CobaltSKY_SOURCE_DIR}/bin/csmoc ${prefix} ${api} ${CMAKE_CURRENT_BINARY_DIR}
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		)

	else()
		set(TARGET_NAME "${target}-MOC")
		add_custom_target(${TARGET_NAME}
				COMMAND ${CobaltSKY_SOURCE_DIR}/bin/csmoc ${prefix} ${api} ${CMAKE_CURRENT_BINARY_DIR}
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		)
		add_dependencies(${target} ${TARGET_NAME})
	endif()
endfunction(CS_MOC)
