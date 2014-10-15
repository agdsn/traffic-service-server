file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto")
file(GLOB message_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})

add_library(messages STATIC ${ProtoSources}
			    ${ProtoHeaders}
			    ${message_SOURCES})

target_link_libraries(messages ${Boost_LIBRARIES} ${PROTOBUF_LIBRARY})

target_include_directories(messages PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
