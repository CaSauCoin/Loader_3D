add_subdirectory(Cube) 
add_subdirectory(Pyramid)
add_subdirectory(Globular)

add_library(Block INTERFACE)

target_link_libraries(Block INTERFACE 
    Cube 
    Pyramid 
    Globular
)