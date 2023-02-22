message(STATUS "----- config qt -----")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(QT_LIBS_TARGET libqt)
add_library(${QT_LIBS_TARGET} INTERFACE)
set(QT_MODULES
        Core
        OpenGL
        Gui
        Qml
        Quick
        QuickControls2
        Multimedia
        WebEngine
        Svg
        Concurrent
        Network
        WebSockets
        )
foreach (qt_module ${QT_MODULES})
    find_package(Qt5${qt_module} REQUIRED)
    target_link_libraries(${QT_LIBS_TARGET} INTERFACE Qt5::${qt_module})
endforeach()

find_package(Qt5LinguistTools REQUIRED)

# ------------------- Auto generate resource.qrc -------------------
file(GLOB_RECURSE IMAGES_PNG_FILES ${CMAKE_SOURCE_DIR}/assets/images/*.png)
file(GLOB_RECURSE IMAGES_PAG_FILES ${CMAKE_SOURCE_DIR}/assets/images/*.pag)
file(GLOB_RECURSE IMAGES_JPG_FILES ${CMAKE_SOURCE_DIR}/assets/images/*.jpg)
file(GLOB_RECURSE IMAGES_SVG_FILES ${CMAKE_SOURCE_DIR}/assets/images/*.svg)
file(GLOB_RECURSE IMAGES_TXT_FILES ${CMAKE_SOURCE_DIR}/assets/images/*.txt)
file(GLOB_RECURSE IMAGES_GIF_FILES ${CMAKE_SOURCE_DIR}/assets/images/*.gif)
file(GLOB_RECURSE SHADERS ${CMAKE_SOURCE_DIR}/assets/shader/*.*)
list(APPEND RESOURCE_FILES
        ${IMAGES_PNG_FILES}
        ${IMAGES_PAG_FILES}
        ${IMAGES_JPG_FILES}
        ${IMAGES_SVG_FILES}
        ${IMAGES_TXT_FILES}
        ${IMAGES_GIF_FILES}
        ${SHADERS}
        )
set(RESOURCE_QRC "${CMAKE_SOURCE_DIR}/assets/resource.qrc")

set(TO_ADD_QRC_FILES "")
# 加入内置的资源文件
list(APPEND TO_ADD_QRC_FILES  "<RCC>\n\t<qresource prefix=\"/\">")
foreach(RES_FILE ${RESOURCE_FILES})
    file(RELATIVE_PATH RELATIVE_PATH ${CMAKE_SOURCE_DIR}/assets ${RES_FILE})
    list(APPEND TO_ADD_QRC_FILES  "\n\t\t<file>${RELATIVE_PATH}</file>")
endforeach()
list(APPEND TO_ADD_QRC_FILES  "\n\t</qresource>\n</RCC>")

file(WRITE ${RESOURCE_QRC} ${TO_ADD_QRC_FILES})
list(APPEND QRC_FILES ${RESOURCE_QRC})
# --------------------------------------------------------------

# ------------------- Auto generate scripts.qrc -------------------
list(APPEND GENERATED_JS_FILES
        editor/index.js
        )

file(GLOB_RECURSE QML_UI_FILES ${CMAKE_SOURCE_DIR}/assets/qml/*.qml)
file(GLOB_RECURSE QML_JS_FILES ${CMAKE_SOURCE_DIR}/assets/qml/*.js)
list(APPEND SCRIPTS_FILES ${QML_UI_FILES} ${QML_JS_FILES})

set(SCRIPTS_QRC "${CMAKE_SOURCE_DIR}/assets/scripts.qrc")
set(TO_ADD_QRC_FILES "")

# 加入内置的js和qml文件
list(APPEND TO_ADD_QRC_FILES  "<RCC>\n\t<qresource prefix=\"/\">")
foreach(RES_FILE ${SCRIPTS_FILES})
    file(RELATIVE_PATH RELATIVE_PATH ${CMAKE_SOURCE_DIR}/assets ${RES_FILE})
    list(APPEND TO_ADD_QRC_FILES  "\n\t\t<file>${RELATIVE_PATH}</file>")
endforeach()

# 加入编译生成的js文件
foreach(JS_FILE ${GENERATED_JS_FILES})
    list(APPEND TO_ADD_QRC_FILES  "\n\t\t<file>${JS_FILE}</file>")
endforeach()
list(APPEND TO_ADD_QRC_FILES  "\n\t</qresource>\n</RCC>")

file(WRITE ${SCRIPTS_QRC} ${TO_ADD_QRC_FILES})
list(APPEND QRC_FILES ${SCRIPTS_QRC})
# --------------------------------------------------------------
