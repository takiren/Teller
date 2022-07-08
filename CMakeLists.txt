cmake_minimum_required(VERSION 3.22)

project(TellerEngine VERSION 0.1.0 LANGUAGES CXX)

set(TELLER_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(TELLER_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proj/cmake")

set(TELLER_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TELLER_H_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(TELLER_EXT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")

set(CINDER_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/Cinder/include")

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(TELLER_TARGET_DEFAULT "msw")
endif()

set(TELLER_TARGET "${TELLER_TARGET_DEFAULT}" CACHE STRING "Target platform to build.")

if(TELLER_TARGET_DEFAULT)
    string(TOLOWER "${TELLER_TARGET}" TELLER_TARGET_LOWER)

    if("msw" STREQUAL "${TELLER_TARGET_LOWER}")
        set(TELLER_MSW TRUE)
    endif()
endif()

if(TELLER_MSW)
    set(TELLER_ARCH "x86")

    if(CMAKE_CL_64)
        set(TELLER_ARCH "x64")
    endif()

    set(TELLER_TARGET_SUBFOLDER "${TELLER_ARCH}")
endif()

if(TELLER_MSW)
    set(PLATFORM_TOOLSET "$(PlatformToolset)")

    if(NOT("${CMAKE_GENERATOR}" MATCHES "Visual Studio.+"))
        # Assume Visual Studio 2019
        set(PLATFORM_TOOLSET "v143")
    endif()

    set(OUTPUT_DIRECTORY_BASE "${TELLER_PATH}/${TELLER_TARGET_SUBFOLDER}")

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY_BASE})
set(WORKING_DIRECTORY ${OUTPUT_DIRECTORY_BASE})

if(TELLER_MSW)
    set(COMPILER_SUPPORTS_CXX17 TRUE)
else()
    message(FATAL_ERROR "Not Windows")
endif()

list(APPEND TELLER_ADDITIONAL_INCLUDE
    ${TELLER_EXT_DIR}/Cinder/include
    ${TELLER_EXT_DIR}/cppglob/include
    ${TELLER_EXT_DIR}/eigen/Eigen
    ${TELLER_EXT_DIR}/json/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CINDER_INC_DIR}/imgui-node-editor
    ${TELLER_EXT_DIR}/imgui-node-editor/examples/blueprints-example
    ${CINDER_INC_DIR}/imgui
)

list(APPEND TELLER_LIBS_DEPENDS
)

list(APPEND TELLER_SRC_FILES
    ${TELLER_SRC_DIR}/Agent.cpp
    ${TELLER_SRC_DIR}/Animation.cpp
    ${TELLER_SRC_DIR}/Asset.cpp
    ${TELLER_SRC_DIR}/Audio.cpp
    ${TELLER_SRC_DIR}/Core.cpp
    ${TELLER_SRC_DIR}/Editor.cpp
    ${TELLER_SRC_DIR}/EditorManager.cpp
    ${TELLER_SRC_DIR}/Episode.cpp
    ${TELLER_SRC_DIR}/EventInstance.cpp
    ${TELLER_SRC_DIR}/Game.cpp
    ${TELLER_SRC_DIR}/InGameUI.cpp
    ${TELLER_SRC_DIR}/main.cpp
    ${TELLER_SRC_DIR}/Renderer.cpp
    ${TELLER_SRC_DIR}/Scene.cpp
    ${TELLER_SRC_DIR}/StoryBoard.cpp
    ${TELLER_SRC_DIR}/TellerCore.cpp
    ${TELLER_SRC_DIR}/TellerEvent.cpp
    ${TELLER_SRC_DIR}/Tree.cpp
    ${TELLER_SRC_DIR}/UIBase.cpp
    ${TELLER_SRC_DIR}/Variant.cpp
    
    ${TELLER_EXT_DIR}/imgui-node-editor/examples/blueprints-example/utilities/builders.cpp
    ${TELLER_EXT_DIR}/imgui-node-editor/examples/blueprints-example/utilities/drawing.cpp
    ${TELLER_EXT_DIR}/imgui-node-editor/examples/blueprints-example/utilities/widgets.cpp
)

list(APPEND TELLER_H_FILES
    ${TELLER_H_DIR}/Agent.h
    ${TELLER_H_DIR}/Animation.h
    ${TELLER_H_DIR}/Asset.h
    ${TELLER_H_DIR}/Audio.h
    ${TELLER_H_DIR}/ContentManager.h
    ${TELLER_H_DIR}/Core.h
    ${TELLER_H_DIR}/Editor.h
    ${TELLER_H_DIR}/EditorManager.h
    ${TELLER_H_DIR}/Episode.h
    ${TELLER_H_DIR}/EventInstance.h
    ${TELLER_H_DIR}/Game.h
    ${TELLER_H_DIR}/InGameUI.h
    ${TELLER_H_DIR}/japaneseGryph.h
    ${TELLER_H_DIR}/NodeEditor.h
    ${TELLER_H_DIR}/NodeEditorPrivate.h
    ${TELLER_H_DIR}/NodeLink.h
    ${TELLER_H_DIR}/Renderer.h
    ${TELLER_H_DIR}/Scene.h
    ${TELLER_H_DIR}/TellerCore.h
    ${TELLER_H_DIR}/TellerEvent.h
    ${TELLER_H_DIR}/ThreadPool.h
    ${TELLER_H_DIR}/Tree.h
    ${TELLER_H_DIR}/UIBase.h
    ${TELLER_H_DIR}/utility.h
    ${TELLER_H_DIR}/Variant.h
)

add_executable(
    TellerEngine
    ${TELLER_SRC_FILES}
    ${TELLER_H_FILES}
)

set_property(TARGET TellerEngine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${OUTPUT_DIRECTORY_BASE}/$<IF:$<CONFIG:Debug>,Debug,Release>")

add_definitions(-D_UNICODE)

if(MSVC)
    add_compile_options(/MP)
    add_compile_options(/std=c++17)
    add_compile_options(/W4)
    set_property(TARGET TellerEngine PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TellerEngine)

    target_link_options(TellerEngine PRIVATE $<$<CONFIG:Debug>:/DEBUG>)
    
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
    add_compile_options(-MP)
    add_compile_options(-std=c++17)
endif()

set(CMAKE_BUILD_TYPE $<IF:$<CONFIG:Debug>,Debug,Release>)

target_compile_definitions(TellerEngine PRIVATE
    $<$<CONFIG:Debug>:WIN32 _DEBUG _WINDOWS NOMINAX>
)

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/NODEFAULTLIB:LIBCMT /NODEFAULTLIB:LIBCPMT")
target_compile_definitions(TellerEngine PRIVATE
    $<$<CONFIG:Release>:WIN32 NDEBUG _WINDOWS NOMINAX _WIN32_WINNT=0x0502>
)

set(CINDER_LIB_PATH "${TELLER_EXT_DIR}/Cinder/lib/msw/x64/${CMAKE_BUILD_TYPE}/v143")

if(COMPILER_SUPPORTS_CXX17)
    if(MSVC_VERSION LESS 1920)
        message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} doesn't have C++17 support. Use a different C++ compiler.")
    endif()

    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" "/std:c++17")

    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" "-std=c++17")
    endif()
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} doesn't have C++17 support. Use a different C++ compiler.")
endif()

target_compile_features(TellerEngine PRIVATE cxx_std_17)

target_include_directories(TellerEngine PRIVATE ${TELLER_ADDITIONAL_INCLUDE})

target_link_directories(TellerEngine PRIVATE ${TELLER_EXT_DIR}/libs)

target_link_libraries(TellerEngine PRIVATE cppglob_static.lib)
target_link_libraries(TellerEngine PRIVATE cinder.lib)

if(MSVC)
    string(REPLACE "/" "\\" OUT_PATH ${OUTPUT_DIRECTORY_BASE})
    add_custom_command(TARGET TellerEngine PRE_BUILD COMMAND xcopy /I /D ${OUT_PATH}\\data ${OUT_PATH}\\$<IF:$<CONFIG:Debug>,Debug,Release>\\data)

else()
    add_custom_command(TARGET TellerEngine PRE_BUILD COMMAND xcopy /I /D ${OUTPUT_DIRECTORY_BASE}/data ${OUTPUT_DIRECTORY_BASE}/$<IF:$<CONFIG:Debug>,Debug,Release>/data)
endif()
