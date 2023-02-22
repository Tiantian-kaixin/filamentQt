cmake_minimum_required(VERSION 3.24)
project(filamentQt)

project(filamentQt LANGUAGES CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(tool)

if (APPLE)
    set_local_qt_path(../Qt/5.13.0/) # 这里需要填入自己安装的路径
elseif (WIN32)
    set_local_qt_path(C:/Qt/Qt5.13.0/5.13.0/)
endif ()

include(qt_config)
if (APPLE AND NOT XCODE)
    set(CMAKE_CXX_FLAGS "-ObjC++ -stdlib=libc++ ${cppFlags}")
endif ()
if (APPLE)
    find_library(MAC_ACCE Accelerate)
    list(APPEND EXTRA_LIBS ${MAC_ACCE})

    if (IOS)
        find_library(UI_KIT UIKit)
        list(APPEND EXTRA_LIBS ${UI_KIT})
    else ()
        find_library(COCOA_LIBRARY Cocoa)
        list(APPEND EXTRA_LIBS ${COCOA_LIBRARY})
    endif ()

    find_library(CODE_VIDEO CoreVideo)
    list(APPEND EXTRA_LIBS ${CODE_VIDEO})

    find_library(CORE_GRAPHICS CoreGraphics)
    list(APPEND EXTRA_LIBS ${CORE_GRAPHICS})

    find_library(METAL_LIBRARY Metal)
    list(APPEND EXTRA_LIBS ${METAL_LIBRARY})

    find_library( CORE_ML_LIBRARY CoreML)
    list(APPEND EXTRA_LIBS ${CORE_ML_LIBRARY})

    find_library( METAL_PERFORMANCE_SHADERS_LIBRARY MetalPerformanceShaders)
    list(APPEND EXTRA_LIBS ${METAL_PERFORMANCE_SHADERS_LIBRARY})

    find_library(AVFOUNDATION AVFoundation)
    list(APPEND EXTRA_LIBS ${AVFOUNDATION})

    message( STATUS "METAL_PERFORMANCE_SHADERS_LIBRARY is ${METAL_PERFORMANCE_SHADERS_LIBRARY}")
endif ()
add_subdirectory(./src/thirdParty/Ace3DEngine)
include_directories(${ACE_3D_ENGINE_INCLUDE_DIR})

file(GLOB CORE_SOURCE_FILES src/*.*)
list(APPEND ALL_SOURCE_FILES
        ${CORE_SOURCE_FILES})
add_executable(${PROJECT_NAME} "main.cpp" ${ALL_SOURCE_FILES} "asset/resource.qrc")

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick ${ACE_3D_LIBS} ${EXTRA_LIBS})
