add_library(Globular STATIC 
    Globular.cpp 
) 

target_include_directories(Globular PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)