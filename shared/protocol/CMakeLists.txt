find_package(Protobuf REQUIRED)
add_library(cpp_proto)

# list proto files you want to build here
set(PROTO_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/schema/can.proto    
)
set(CPP_PROTO_OUT_DIR 
    ${CMAKE_BINARY_DIR}/shared/generated/cpp
)
set(PYTHON_PROTO_OUT_DIR
    ${CMAKE_SOURCE_DIR}/odyssey/src/odyssey/protocol
)

protobuf_generate(
    TARGET cpp_proto
    LANGUAGE cpp
    PROTOS ${PROTO_FILES}
    IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/schema
    PROTOC_OUT_DIR ${CPP_PROTO_OUT_DIR}
)

protobuf_generate(
    OUT_VAR PY_PROTO_SRCS
    LANGUAGE python
    PROTOS ${PROTO_FILES}
    IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/schema
    PROTOC_OUT_DIR ${PYTHON_PROTO_OUT_DIR}
)

target_include_directories(cpp_proto PUBLIC ${CPP_PROTO_OUT_DIR})
target_link_libraries(cpp_proto PUBLIC protobuf::libprotobuf)

add_custom_target(py_proto ALL DEPENDS ${PY_PROTO_SRCS})
