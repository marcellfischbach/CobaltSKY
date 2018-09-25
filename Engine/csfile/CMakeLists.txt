

add_definitions(-DCSF_EXPORT)
add_definitions(-D_USE_MATH_DEFINES)


set(SOURCE
	csfblob.cc
	csfblob.hh
    csfexport.hh
    csffile.hh
    csffile.cc
    csfentry.cc
    csfentry.hh
    )

source_group(
    "csfile"
    FILES
    ${SOURCE}
    )


set(ALL_SOURCES
		${SOURCE}
		)

add_library(CobaltSKY-File SHARED
    ${ALL_SOURCES}
    )


install(TARGETS CobaltSKY-File 
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		PUBLIC_HEADER DESTINATION include/cobaltsky/csfile
		)

install(DIRECTORY . DESTINATION include/cobaltsky/csfile FILES_MATCHING PATTERN "*.hh")
