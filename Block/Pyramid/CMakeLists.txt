add_library(Pyramid STATIC 
    Pyramid.cpp 
) 

target_include_directories(Pyramid PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)