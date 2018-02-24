PROJECT (swiftcpp)
SET(SRC_LIST swiftcpp/io/any_vector_compressor.cpp)
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(swiftcpp STATIC ${SRC_LIST})
install(CODE "MESSAGE(\"hehe\")")
