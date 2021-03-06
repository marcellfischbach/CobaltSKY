

function(CS_MOC trgt prefix)

	set(MOC_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/__cmake__build__moc__")

	set(FILENAME "${MOC_DIRECTORY}/${prefix}/.csmoc")
	file(REMOVE "${FILENAME}")

	set (EXEC_PATH "")
	if (CobaltSKY_BINARY_DIR) 
		set(EXEC_PATH "${CobaltSKY_BINARY_DIR}/bin/")
	endif()
	


	string(MAKE_C_IDENTIFIER ${trgt} TARGET_IDENTIFIER)
	add_definitions(-D${TARGET_IDENTIFIER})

	get_target_property(ALL_SOURCES ${trgt} SOURCES)
	foreach(file ${ALL_SOURCES})
		file(APPEND "${FILENAME}" ${file} "\n")
	endforeach(file)

	set(TARGET_NAME "${trgt}-MOC")
	add_custom_target(${TARGET_NAME}
                            COMMAND ${EXEC_PATH}csmoc --prefix ${prefix}  --path ${MOC_DIRECTORY}/${prefix} --export ${TARGET_IDENTIFIER}
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
	if (CobaltSKY_BINARY_DIR) 
		# this is an in-engine build
		add_dependencies(${TARGET_NAME} csmoc)
	endif()
	add_dependencies(${trgt} ${TARGET_NAME})


	target_include_directories(${trgt} PUBLIC ${MOC_DIRECTORY})

	
endfunction(CS_MOC)
