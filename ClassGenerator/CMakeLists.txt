

add_executable(ClassGenerator
  cache.cc
  cache.hh
	class.cc
	class.hh
  generator.cc
	main.cc
  reader.cc
  reader.hh
	sourcefile.cc
	sourcefile.hh
	tokenizer.cc
	tokenizer.hh
	)


 install(TARGETS ClassGenerator DESTINATION  ${CobaltSKY_SOURCE_DIR}/bin)
