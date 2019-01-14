
# Use the package PkgConfig to detect GTK+ headers/library files
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtkmm-3.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
include_directories(${ASSIMP_INCLUDE_DIR})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})


# Add other flags to the compiler
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

add_definitions(-DEDITOR_CORE_EXPORT)





SET(SOURCES
        editor.cc
        editor.hh
        iview.hh
	main.cc
	)

SET(ASSETMANAGER_SOURCES
        assetmanager/assetmanagerview.cc
        assetmanager/assetmanagerview.hh
        )

SET(MAINWINDOW_SOURCES
        mainwindow/mainwindow.cc
        mainwindow/mainwindow.hh
	)


SET(ALL_SOURCES
	${SOURCES}
        ${ASSETMANAGER_SOURCES}
	${MAINWINDOW_SOURCES}
	)

# Add an executable compiled from hello.c
ADD_EXECUTABLE(EditorCore 
	${ALL_SOURCES})

# Link the target to the GTK+ libraries
TARGET_LINK_LIBRARIES(EditorCore 
	${GTK3_LIBRARIES}
    CobaltSKY
    CobaltSKY-GraphicsGL4
    CobaltSKY-PhysicsBullet
    ${assimp_LIBRARIES}
	)
	
CS_INSTALL_MOCCER(EditorCore editorcore EDITOR_CORE_API ALL_SOURCES)
	


