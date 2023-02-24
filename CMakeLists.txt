cmake_minimum_required(VERSION 3.20.0)
project(open_gl VERSION 0.1.0)

include(CTest)
enable_testing()

#отключение документации, тестов, примеров GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(./depends/GLFW)


# добавление путей файлов
include_directories(
    ./include/
    ./src/
    ./src/shaders/
    ./depends/
    ./depends/glad/
    ./depends/GLFW/
    $ENV{VULKAN_SDK}/Include/
)

# Перебор файлов шейдоров и их компиляция
file(GLOB_RECURSE SHADERS CONFIGURE_DEPENDS src/shaders/*.glsl)
foreach(shader IN ITEMS  ${SHADERS})
    get_filename_component(file_name ${shader} NAME_WLE)
    add_custom_command(
        OUTPUT data/${file_name}.spv
        DEPENDS ${shader} 
        COMMAND $ENV{VULKAN_SDK}/Bin/glslangValidator -l -V ${shader} -o data/${file_name}.spv
    )
    
    list(APPEND SHADERS_RESULTS data/${file_name}.spv)
endforeach(shader IN ITEMS ${SHADERS})

# Добавление в контекст данных
add_custom_target(
    Data_Context ALL
    DEPENDS ${SHADERS_RESULTS}
)

# Перебор исходных файлов и их компиляция
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
    src/*.cpp 
    ./depends/glad/glad.c
)

add_executable(open_gl ${SOURCES} main.cpp)

# добавить библиотеки
target_link_libraries(open_gl glfw)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
