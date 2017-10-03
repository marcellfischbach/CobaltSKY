

add_definitions(-DCSF_EXPORT)
add_definitions(-DCS_BUILD_EDITOR)
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



