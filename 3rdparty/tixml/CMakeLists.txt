

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})



set(
	TIXML_SOURCE
  tinystr.cpp
  tinystr.h
  tinyxml.cpp
  tinyxml.h
  tinyxmlerror.cpp
  tinyxmlparser.cpp

	)


add_library(
  TiXml STATIC
  ${TIXML_SOURCE}
  )

		


