cmake_minimum_required(VERSION 3.5)

add_subdirectory(Cocos2dxViaQt)

# CoreCocos2d root path
target_include_directories(Cocos2dxViaQt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
