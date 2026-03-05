cmake_minimum_required(VERSION 3.20)

# Run: cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/Tools/vcpkg/scripts/buildsystems/vcpkg.cmake

set(CMAKE_TOOLCHAIN_FILE "C:/Tools/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "vcpkg toolchain file")

project(vkspace)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>)

set(SHADER_SRC_DIR "${CMAKE_SOURCE_DIR}/src/shaders")
set(SHADER_DST_DIR "${CMAKE_BINARY_DIR}/shaders")

set(TEX_SRC_DIR "${CMAKE_SOURCE_DIR}/assets/textures")
set(TEX_DST_DIR "${CMAKE_BINARY_DIR}/assets/textures")

find_package(PkgConfig REQUIRED)

find_package(glfw3 CONFIG REQUIRED)

find_package(Vulkan REQUIRED)

find_package(ktx CONFIG REQUIRED)

file(GLOB_RECURSE SOURCE_FILES CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx")

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

target_compile_features(vkspace PRIVATE cxx_std_20)

target_include_directories(${CMAKE_PROJECT_NAME}
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PUBLIC
    glfw
    Vulkan::Vulkan
    KTX::ktx
)

# Shader compilation
find_program(GLSLANG_VALIDATOR glslangValidator REQUIRED)

file(GLOB SHADER_FILES "${SHADER_SRC_DIR}/*.vert" "${SHADER_SRC_DIR}/*.frag")

set(SPIRV_OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders)

set(SPIRV_OUTPUT_FILES "")
file(MAKE_DIRECTORY ${SHADER_DST_DIR})

foreach(SHADER ${SHADER_FILES})
    get_filename_component(FILENAME ${SHADER} NAME)
    set(SPIRV_FILE ${SHADER_DST_DIR}/${FILENAME}.spv)
    
    add_custom_command(
        OUTPUT ${SPIRV_FILE}
        COMMAND ${GLSLANG_VALIDATOR} -V ${SHADER} -o ${SPIRV_FILE}
        DEPENDS ${SHADER}
        COMMENT "Compiling ${SHADER} to ${SPIRV_FILE}"
        VERBATIM
    )
    
    list(APPEND SPIRV_OUTPUT_FILES ${SPIRV_FILE})
endforeach()

add_custom_target(Shaders ALL DEPENDS ${SPIRV_OUTPUT_FILES})

# Image conversion
find_program(KTXTOOLS ktx REQUIRED)

file(GLOB SOURCE_TEXTURES "${TEX_SRC_DIR}/*.png")

set(TEX_OUTPUT_DIR ${CMAKE_BINARY_DIR}/assets/textures)

set(TEX_OUTPUT_FILES "")
file(MAKE_DIRECTORY ${TEX_DST_DIR})

foreach(INPUT_TEXTURE ${SOURCE_TEXTURES})
    get_filename_component(FILENAME ${INPUT_TEXTURE} NAME_WE)
    set(OUTPUT_TEXTURE ${TEX_OUTPUT_DIR}/${FILENAME}.ktx2)

    add_custom_command(
        OUTPUT  ${OUTPUT_TEXTURE}
        COMMAND ${KTXTOOLS} create
            --format R8G8B8A8_UNORM
            ${INPUT_TEXTURE}
            ${OUTPUT_TEXTURE}
        DEPENDS ${INPUT_TEXTURE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Converting ${INPUT_TEXTURE} to .ktx2"
    )

    list(APPEND TEX_OUTPUT_FILES ${OUTPUT_TEXTURE})
endforeach()

add_custom_target(Textures ALL DEPENDS ${TEX_OUTPUT_FILES})

add_dependencies(vkspace Textures)
add_dependencies(vkspace Shaders)