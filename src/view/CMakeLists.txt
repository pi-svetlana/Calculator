set(target view)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS PrintSupport)

set(PROJECT_SOURCES
    main.cc
    view.cc
    view.h
    view.ui
    qcustomplot.cpp
    qcustomplot.h
)

set(THIRDPARTY_SOURCES
    qcustomplot.h
    qcustomplot.cpp
)

add_library(
    QCustomPlot
    ${THIRDPARTY_SOURCES}
)

set_target_properties(
  QCustomPlot PROPERTIES
  LINKER_LANGUAGE CXX
)

target_link_libraries(QCustomPlot Qt${QT_VERSION_MAJOR}::Widgets)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${target}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET view APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${target} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${target}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

#target_link_libraries(view PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(${target} PRIVATE QCustomPlot Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::PrintSupport
    model
    controller)

set_target_properties(${target} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${target})
endif()
