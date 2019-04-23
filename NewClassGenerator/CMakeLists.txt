

cmake_minimum_required(VERSION 3.3)

project(NewClassGenerator)

include_directories(${NewClassGenerator_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(NewClassGenerator
	ast.cc
	ast.hh
	main.cc
	generate/generator.cc
	generate/generator.hh
	generate/header.cc
	generate/header.hh
	generate/source.cc
	generate/source.hh
	parser/parseexception.hh
	parser/parser.cc
	parser/parser.hh
	parser/sourcefile.cc
	parser/sourcefile.hh
	parser/token.cc
	parser/token.hh
	parser/tokenizer.cc
	parser/tokenizer.hh
	)


