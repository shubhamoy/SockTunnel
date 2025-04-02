cmake_minimum_required(VERSION 3.16)
project(SockTunnel VERSION 1.0 LANGUAGES CXX)

# Set up Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets SvgWidgets Network DBus)

# Gather all sources
file(GLOB_RECURSE SOURCES
    src/main.cpp
    src/resources.cpp
    src/app/*.cpp
    src/core/*.cpp
    src/ui/*.cpp
)

# Add executable target
qt_add_executable(SockTunnel 
	${SOURCES}
)

# Link Qt dependencies
target_link_libraries(SockTunnel PRIVATE Qt6::Widgets Qt6::SvgWidgets Qt6::Network Qt6::DBus)

# Include directories
target_include_directories(SockTunnel PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src    
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
)

# macOS bundle settings
if(APPLE)
    set_target_properties(SockTunnel PROPERTIES MACOSX_BUNDLE TRUE)
    set_target_properties(SockTunnel PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER "com.shubhamoy.socktunnel")
    set(MACOSX_BUNDLE_ICON_FILE icon.icns)
    target_sources(SockTunnel PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/icon.icns)
    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/resources/icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    set_target_properties(SockTunnel PROPERTIES 
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "@executable_path/../Frameworks"
    )
    install(TARGETS SockTunnel
        BUNDLE DESTINATION .
        RUNTIME DESTINATION .
    )
endif()
