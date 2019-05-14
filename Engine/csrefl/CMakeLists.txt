

add_definitions(-DCSREFL_EXPORT)


add_library(CobaltREFL SHARED
  class.hh
  class.cc
  classregistry.hh
  classregistry.cc
  module.cc
  module.hh
  )


install(TARGETS CobaltREFL
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		)

install(DIRECTORY . DESTINATION include/cobaltsky/csrefl FILES_MATCHING PATTERN "*.hh")

