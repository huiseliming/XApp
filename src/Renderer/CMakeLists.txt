
set(IMGUI_DOCKING_VER 1.89.8)
set(IMGUI_DOCKING_ZIP imgui-${IMGUI_DOCKING_VER}-docking.zip)
set(IMGUI_DOCKING_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui-${IMGUI_DOCKING_VER}-docking)
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/imgui-${IMGUI_DOCKING_VER}-docking.zip")
    file(DOWNLOAD
        "https://github.com/ocornut/imgui/archive/refs/tags/v${IMGUI_DOCKING_VER}-docking.zip"
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui-${IMGUI_DOCKING_VER}-docking.zip
        SHOW_PROGRESS
    )
    execute_process(COMMAND
        ${CMAKE_COMMAND} -E tar xzf ${CMAKE_CURRENT_SOURCE_DIR}/imgui-${IMGUI_DOCKING_VER}-docking.zip
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

set(CURRENT_MODULE_NAME "XRenderer")

file(GLOB ${CURRENT_MODULE_NAME}_HEADERS *.h *.hh *.hpp *.hxx)
file(GLOB ${CURRENT_MODULE_NAME}_SOURCES *.c *.cc *.cpp *.cxx)
file(GLOB_RECURSE ${CURRENT_MODULE_NAME}_IMPL_HEADERS OpenGL/*.h Vulkan/*.h)
file(GLOB_RECURSE ${CURRENT_MODULE_NAME}_IMPL_SOURCES OpenGL/*.cpp Vulkan/*.cpp)

source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR} 
  PREFIX src
  FILES ${${CURRENT_MODULE_NAME}_HEADERS} ${${CURRENT_MODULE_NAME}_SOURCES} ${${CURRENT_MODULE_NAME}_IMPL_HEADERS} ${${CURRENT_MODULE_NAME}_IMPL_SOURCES}
  ${IMGUI_DOCKING_DIR}/imgui.cpp
  ${IMGUI_DOCKING_DIR}/imgui_demo.cpp
  ${IMGUI_DOCKING_DIR}/imgui_draw.cpp
  ${IMGUI_DOCKING_DIR}/imgui_tables.cpp
  ${IMGUI_DOCKING_DIR}/imgui_widgets.cpp
  ${IMGUI_DOCKING_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DOCKING_DIR}/backends/imgui_impl_opengl3.cpp
  ${IMGUI_DOCKING_DIR}/backends/imgui_impl_vulkan.cpp
  ${IMGUI_DOCKING_DIR}/misc/cpp/imgui_stdlib.cpp
)
# library target 
add_library(${CURRENT_MODULE_NAME} SHARED ${${CURRENT_MODULE_NAME}_HEADERS} ${${CURRENT_MODULE_NAME}_SOURCES} ${${CURRENT_MODULE_NAME}_IMPL_HEADERS} ${${CURRENT_MODULE_NAME}_IMPL_SOURCES}
  ${IMGUI_DOCKING_DIR}/imgui.cpp
  ${IMGUI_DOCKING_DIR}/imgui_demo.cpp
  ${IMGUI_DOCKING_DIR}/imgui_draw.cpp
  ${IMGUI_DOCKING_DIR}/imgui_tables.cpp
  ${IMGUI_DOCKING_DIR}/imgui_widgets.cpp
  ${IMGUI_DOCKING_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DOCKING_DIR}/backends/imgui_impl_opengl3.cpp
  ${IMGUI_DOCKING_DIR}/backends/imgui_impl_vulkan.cpp
  ${IMGUI_DOCKING_DIR}/misc/cpp/imgui_stdlib.cpp
)
# include directories
target_include_directories(${CURRENT_MODULE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${IMGUI_DOCKING_DIR})
# link libraries
target_link_libraries(${CURRENT_MODULE_NAME} PUBLIC XCore)
# glfw
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${CURRENT_MODULE_NAME} PUBLIC glfw)
# opengl
find_package(glad CONFIG REQUIRED)
target_link_libraries(${CURRENT_MODULE_NAME} PRIVATE glad::glad)
# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${CURRENT_MODULE_NAME} PUBLIC Vulkan::Vulkan)
find_package(unofficial-vulkan-memory-allocator CONFIG REQUIRED)
target_link_libraries(${CURRENT_MODULE_NAME} PRIVATE unofficial::vulkan-memory-allocator::vulkan-memory-allocator)
# export
include(GenerateExportHeader)
string(TOUPPER ${CURRENT_MODULE_NAME} CK_MODULE_NAME_UPPER)
generate_export_header(${CURRENT_MODULE_NAME}
    EXPORT_MACRO_NAME ${CK_MODULE_NAME_UPPER}_API
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/${CURRENT_MODULE_NAME}Export.h
)
target_compile_definitions(${CURRENT_MODULE_NAME} PUBLIC -DIMGUI_API=${CK_MODULE_NAME_UPPER}_API)
# precompile header
target_precompile_headers(${CURRENT_MODULE_NAME} PUBLIC 
    ${CMAKE_CURRENT_BINARY_DIR}/${CURRENT_MODULE_NAME}Export.h
)

# set_target_properties(${CURRENT_MODULE_NAME} PROPERTIES XCPP_CODE_GENERATOR_TARGETS ${CMAKE_CURRENT_SOURCE_DIR}/Object.h)

# #define_property(TARGET PROPERTY XCPP_CODE_GENERATOR_TARGETS)
# set_target_properties(${CURRENT_MODULE_NAME} PROPERTIES XCPP_CODE_GENERATOR_TARGETS )
# get_target_property(var ${CURRENT_MODULE_NAME} XCPP_CODE_GENERATOR_TARGETS)
# message("var": ${var})

# if(NOT CMAKE_PROPERTY_LIST)
#     execute_process(COMMAND cmake --help-property-list OUTPUT_VARIABLE CMAKE_PROPERTY_LIST)
    
#     # Convert command output into a CMake list
#     string(REGEX REPLACE ";" "\\\\;" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
#     string(REGEX REPLACE "\n" ";" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
#     list(REMOVE_DUPLICATES CMAKE_PROPERTY_LIST)
# endif()
    
# function(print_properties)
#     message("CMAKE_PROPERTY_LIST = ${CMAKE_PROPERTY_LIST}")
# endfunction()
    
# function(print_target_properties target)
#     if(NOT TARGET ${target})
#       message(STATUS "There is no target named '${target}'")
#       return()
#     endif()

#     foreach(property ${CMAKE_PROPERTY_LIST})
#         string(REPLACE "<CONFIG>" "${CMAKE_BUILD_TYPE}" property ${property})

#         # Fix https://stackoverflow.com/questions/32197663/how-can-i-remove-the-the-location-property-may-not-be-read-from-target-error-i
#         if(property STREQUAL "LOCATION" OR property MATCHES "^LOCATION_" OR property MATCHES "_LOCATION$")
#             continue()
#         endif()

#         get_property(was_set TARGET ${target} PROPERTY ${property} SET)
#         if(was_set)
#             get_target_property(value ${target} ${property})
#             message("${target} ${property} = ${value}")
#         endif()
#     endforeach()
# endfunction()

# print_target_properties(${CURRENT_MODULE_NAME})
