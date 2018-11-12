
# Use the package PkgConfig to detect GTK+ headers/library files
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtkmm-3.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})


SET(EDITOR_SOURCES
	main.cc
        editor.cc
        editor.hh
        mainwindow.cc
        mainwindow.hh
	)


SET(ALL_SOURCES
	${EDITOR_SOURCES}
	)


ADD_EXECUTABLE(editor 
	${ALL_SOURCES})

# Link the target to the GTK+ libraries
TARGET_LINK_LIBRARIES(editor ${GTK3_LIBRARIES})
