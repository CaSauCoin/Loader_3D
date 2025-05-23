add_executable(Loader3D 
    main.cpp 
    ${IMGUI_SOURCES}
) 

target_include_directories(Loader3D PRIVATE 
    ${CMAKE_SOURCE_DIR}/Common/include 
) 

target_link_libraries(Loader3D PRIVATE 
    Common 
    Cube 
    Pyramid 
    Globular
    OpenGL::GL 
    GLEW::GLEW 
    glfw 
    glm::glm 
    dl
) 

target_compile_features(Loader3D PRIVATE cxx_std_17)