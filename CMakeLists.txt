cmake_minimum_required(VERSION 3.10) 
project(Loader3D)

set(CMAKE_CXX_STANDARD 17)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

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
    install_package("libglfw3-dev" glfw3)
    install_package("libglew-dev" GLEW)
    install_package("libglm-dev" glm)
endif()

if(IS_MACOS)
    install_package("glfw" glfw3)
    install_package("glew" GLEW)
    install_package("glm" glm)
endif()

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

set(IMGUI_SOURCES 
    ${IMGUI_SOURCES} 
    CACHE INTERNAL "ImGui source files"
    )

include_directories( 
    ${CMAKE_SOURCE_DIR}/Block/Cube 
    ${CMAKE_SOURCE_DIR}/Block/Pyramid 
    ${CMAKE_SOURCE_DIR}/Block/Globular
    ${CMAKE_SOURCE_DIR}/Common/include 
    ${CMAKE_SOURCE_DIR}/Utils/include 
)

add_subdirectory(Block) 
add_subdirectory(Common) 
add_subdirectory(Utils) 
add_subdirectory(Src)
