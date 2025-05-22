
set(SOURCES
    main.cpp
    Cube.cpp
    Pyramid.cpp
    Renderer.cpp
    Controller.cpp
    Application.cpp
)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)
if(NOT EXISTS ${IMGUI_DIR}/imgui.h)
    message(STATUS "imgui not found or empty. Cloning...")
    execute_process(
        COMMAND git clone https://github.com/ocornut/imgui.git ${IMGUI_DIR}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE GIT_RESULT
    )
    if(NOT GIT_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to clone imgui")
    endif()
endif()

file(GLOB IMGUI_SOURCES
    "${PROJECT_SOURCE_DIR}/imgui/*.cpp"
    "${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
)

add_executable(RubiksCube
    ${SOURCES}
    ${IMGUI_SOURCES}
)

target_include_directories(RubiksCube PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW3_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/imgui
    ${PROJECT_SOURCE_DIR}/imgui/backends
)

target_link_libraries(RubiksCube
    ${OPENGL_LIBRARIES}
    GLEW::GLEW
    glfw
    dl
)

target_compile_features(RubiksCube PRIVATE cxx_std_17)

