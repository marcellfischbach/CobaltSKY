

set(SOURCE
  Test.hh
  TestInterface.hh
  Types.hh
  main.cc
  )

source_group(
	"ValkyrieEngine"
	FILES
	${SOURCE}
	)

set(SOURCE_CORE
	Core/Object.cc
	Core/Object.hh
	)

source_group(
	"ValkyrieEngine\\Core"
	FILES
	${SOURCE_CORE}
	)

set(ALL_SOURCES
  ${SOURCE}
	${SOURCE_CORE}
	)

VK_MOC(Valkyrie ALL_SOURCES MOC_SOURCES)


source_group(
	"ValkyrieEngine\\MOC_sources"
	FILES
	${MOC_SOURCES}
	)

add_executable(ValkyrieEngine
	${ALL_SOURCES}
	${MOC_SOURCES}
	)

VK_INSTALL_MOCCER(ValkyrieEngine Valkyrie ALL_SOURCES)

