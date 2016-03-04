
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5OpenGL)



set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

set(EDITOR_SOURCES
	main.cc
  Editor.cc
  Editor.hh
  EditorWindow.cc
  EditorWindow.hh
  SceneView.cc
  SceneView.hh
	)

source_group(
	"Editor"
	FILES
	${EDITOR_SOURCES}
	)



set(EDITOR_SHADERGRAPH_SOURCES
    ShaderGraph/SGToolBox.cc
    ShaderGraph/SGToolBox.hh
    ShaderGraph/ShaderGraphView.cc
    ShaderGraph/ShaderGraphView.hh
    )

source_group(
	"Editor\\ShaderGraph"
	FILES
	${EDITOR_SHADERGRAPH_SOURCES}
	)


	
	

add_executable(
  ValkyrieEditor 
  ${EDITOR_SOURCES}
	${EDITOR_SHADERGRAPH_SOURCES}
  )

target_link_libraries(ValkyrieEditor 
    Qt5::Core
    Qt5::Widgets 
    Qt5::OpenGL
    ValkyrieEngine
    ValkyrieEngine-GraphicsGL4
    ValkyrieEngine-PhysicsBullet
    )

