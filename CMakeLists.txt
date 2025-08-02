cmake_minimum_required(VERSION 3.21.1)

option(LINK_INSIGHT "Link Qt Insight Tracker library" ON)
option(BUILD_QDS_COMPONENTS "Build design studio components" ON)

project(CalculatorApp LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(UI_DIR "src/UI")

find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Qml Quick)

if(Qt6_VERSION VERSION_GREATER_EQUAL 6.3)
    qt_standard_project_setup()
endif()

qt_add_executable(CalculatorApp
    src/data_structs.h
                                        src/main.cpp
    src/controller.h                    src/controller.cpp
    src/Modules/expression_processor.h  src/Modules/expression_processor.cpp
    src/thread_safe_qqueue.h
    src/UI/main_window.h                src/UI/main_window.cpp
)


file(GLOB_RECURSE QML_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UI/CalculatorQML/*.qml
)

foreach(qml_file IN LISTS QML_FILES)
    # Устанавливаем относительный путь в ресурсах
    file(RELATIVE_PATH rel_path "${CMAKE_CURRENT_SOURCE_DIR}/src/UI/CalculatorQML" "${qml_file}")
    set_source_files_properties(${qml_file} PROPERTIES QT_RESOURCE_ALIAS "${rel_path}")
endforeach()

message(STATUS "QML FILES FOUND:")
foreach(qmlfile ${QML_FILES})
    message(STATUS "  ${qmlfile}")
endforeach()

qt_add_qml_module(CalculatorApp
    URI CalculatorAppQml.UI
    VERSION 1.0
    QML_FILES ${QML_FILES}
    RESOURCE_PREFIX /qml
    NO_RESOURCE_TARGET_PATH
)

target_link_libraries(CalculatorApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
)
