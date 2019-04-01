

cmake_minimum_required(VERSION 3.3)

project(NewClassGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(NewClassGenerator
	main.cc
	namespace.cc
	namespace.hh
	sourcefile.cc
	sourcefile.hh
	tokenizer.cc
	tokenizer.hh
	)


