add_library(Common STATIC 
    src/Application.cpp 
    src/Renderer.cpp 
    src/Controller.cpp 
) 

target_include_directories(Common PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
    ${PROJECT_SOURCE_DIR}/imgui 
    ${PROJECT_SOURCE_DIR}/imgui/backends 
)

target_link_libraries(Common PUBLIC 
    Cube 
    Pyramid 
    OpenGL::GL 
    GLEW::GLEW 
    glfw 
    glm::glm 
    dl
)
