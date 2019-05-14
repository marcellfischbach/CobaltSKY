

function(CS_MOC trgt prefix)

	set(FILENAME "${CMAKE_CURRENT_BINARY_DIR}/.csmoc")
	message("MocFileName: ${FILENAME}")
	file(REMOVE "${FILENAME}")

	get_target_property(ALL_SOURCES ${trgt} SOURCES)

	string(MAKE_C_IDENTIFIER ${trgt} TARGET_IDENTIFIER)
	message("Target Identifier: '${TARGET_IDENTIFIER}'")

	set (EXEC_PATH "")
	if (CobaltSKY_BINARY_DIR) 
		set(EXEC_PATH "${CobaltSKY_BINARY_DIR}/bin/")
	endif()

	add_definitions(-D${TARGET_IDENTIFIER})
	foreach(file ${ALL_SOURCES})
		file(APPEND "${FILENAME}" ${file} "\n")
	endforeach(file)
	if (MSVC_IDE)
		message("Generate for MSVC-IDE")
		add_custom_command (TARGET ${trgt} PRE_BUILD
                                COMMAND ${EXEC_PATH}csmoc --prefix ${prefix} --path ${CMAKE_CURRENT_BINARY_DIR} --export ${TARGET_IDENTIFIER}
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		)
		if (CobaltSKY_BINARY_DIR) 
			# this is an in-engine build
			add_dependencies(${trgt} csmoc)
		endif()

	else()
		message("Generate for non-MSVC-IDE")
		set(TARGET_NAME "${trgt}-MOC")
		add_custom_target(${TARGET_NAME}
                                COMMAND ${EXEC_PATH}csmoc --prefix ${prefix}  --path ${CMAKE_CURRENT_BINARY_DIR} --export ${TARGET_IDENTIFIER}
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		)
		if (CobaltSKY_BINARY_DIR) 
			# this is an in-engine build
			add_dependencies(${TARGET_NAME} csmoc)
		endif()
		add_dependencies(${trgt} ${TARGET_NAME})
	endif()
endfunction(CS_MOC)