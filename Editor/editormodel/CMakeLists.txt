


set(ALL_SOURCES
	editormodelexport.hh
    model.cc
    model.hh
	nodescanner.cc
	nodescanner.hh
	modelexceptions.hh
	pathscanner.cc
	pathscanner.hh
    transaction.cc
    transaction.hh
	fs/secfs.cc
	fs/secfs.hh
    nodes/assetnode.cc
    nodes/assetnode.hh
    nodes/foldernode.cc
    nodes/foldernode.hh
    nodes/node.cc
    nodes/node.hh
    nodes/rootnode.cc
    nodes/rootnode.hh
    nodes/vfsentrynode.cc
    nodes/vfsentrynode.hh
    )


add_definitions(-DEDITOR_MODEL_EXPORT)


add_library(
    EditorModel
	SHARED
    ${ALL_SOURCES}
    )
	
add_dependencies(EditorModel ClassGenerator)

target_link_libraries(EditorModel
    CobaltSKY
	CobaltSKY-File
    )

CS_INSTALL_MOCCER(EditorModel editormodel EDITOR_MODEL_API ALL_SOURCES)




add_executable(
        EditorModelTest
        main.cc
)


add_dependencies(EditorModelTest ClassGenerator)

target_link_libraries(
        EditorModelTest
        EditorModel
)
