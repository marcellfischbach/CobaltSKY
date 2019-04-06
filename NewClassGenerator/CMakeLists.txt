

cmake_minimum_required(VERSION 3.3)

project(NewClassGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(NewClassGenerator
	codenodes.cc
	codenodes.hh
	main.cc
	parser.cc
	parser.hh
	sourcefile.cc
	sourcefile.hh
	token.cc
	token.hh
	tokenizer.cc
	tokenizer.hh
	)


