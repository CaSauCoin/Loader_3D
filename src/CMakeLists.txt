
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

# Detect platform
if(UNIX AND NOT APPLE)
    set(IS_LINUX TRUE)
elseif(APPLE)
    set(IS_MACOS TRUE)
endif()

function(install_package pkg cmake_package)
    find_package(${cmake_package} QUIET)
    if(NOT ${cmake_package}_FOUND)
        if(IS_LINUX)
            message(STATUS "Installing ${pkg} using apt...")
            execute_process(
                COMMAND bash -c "dpkg -s ${pkg} &> /dev/null || sudo apt-get update && sudo apt-get install -y ${pkg}"
                RESULT_VARIABLE res
            )
        elseif(IS_MACOS)
            message(STATUS "Installing ${pkg} using brew...")
            execute_process(
                COMMAND brew list ${pkg}
                RESULT_VARIABLE found
            )
            if(NOT found EQUAL 0)
                execute_process(
                    COMMAND brew install ${pkg}
                    RESULT_VARIABLE res
                )
            endif()
        endif()
        # Try again after installation
        find_package(${cmake_package} REQUIRED)
    else()
        message(STATUS "${cmake_package} already found, skipping install.")
    endif()
endfunction()

if(IS_LINUX)
    install_package("libglfw3dev" glfw3)
    install_package("libglewdev" GLEW)
    install_package("libglmdev" glm)
endif()

if(IS_MACOS)
    install_package("glfw" glfw3)
    install_package("glew" GLEW)
    install_package("glm" glm)
endif()

file(GLOB IMGUI_SOURCES
    "${PROJECT_SOURCE_DIR}/imgui/*.cpp"
    "${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
)

add_executable(Loader3D
    ${SOURCES}
    ${IMGUI_SOURCES}
)

target_include_directories(Loader3D PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW3_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/imgui
    ${PROJECT_SOURCE_DIR}/imgui/backends
)

target_link_libraries(Loader3D
    ${OPENGL_LIBRARIES}
    GLEW::GLEW
    glfw
    dl
)

target_compile_features(Loader3D PRIVATE cxx_std_17)

