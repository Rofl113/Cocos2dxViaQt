cmake_minimum_required(VERSION 3.5)

set(HEADERS_Cocos2dxViaQt
    ./GLViewQt.h
    ./QtWidgetCocos2d.h
    )

set(SOURCES_Cocos2dxViaQt
    ./GLViewQt.cpp
    ./QtWidgetCocos2d.cpp
    )

# Next lines needed for building all Qt projects
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# Создание статической библиотеки
add_library(Cocos2dxViaQt STATIC ${SOURCES_Cocos2dxViaQt} ${HEADERS_Cocos2dxViaQt})

target_link_libraries(Cocos2dxViaQt PUBLIC
    Qt5::Core
    Qt5::Widgets
    CoreCocos2d_LIB)
