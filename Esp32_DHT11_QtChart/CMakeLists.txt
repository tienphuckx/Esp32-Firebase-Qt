cmake_minimum_required(VERSION 3.16)
find_package(Vulkan REQUIRED)


# Find Vulkan package or specify manually
find_package(Vulkan REQUIRED)

if (Vulkan_FOUND)
    message(STATUS "Found Vulkan: ${Vulkan_INCLUDE_DIR}")
else()
    # Manually set Vulkan include path if not found automatically
    set(Vulkan_INCLUDE_DIR "C:/VulkanSDK/1.3.290.0/Include")
    include_directories(${Vulkan_INCLUDE_DIR})

    # Link Vulkan library (you may need to specify the library path if not auto-found)
    set(Vulkan_LIBRARY "C:/VulkanSDK/1.3.290.0/Lib/vulkan-1.lib")
    target_link_libraries(appEsp32_DHT11_QtChart PRIVATE ${Vulkan_LIBRARY})
endif()




project(Esp32_DHT11_QtChart VERSION 0.1 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "C:/Qt/6.5.3/msvc2019_64")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Charts)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appEsp32_DHT11_QtChart
    main.cpp
)

qt_add_qml_module(appEsp32_DHT11_QtChart
    URI Esp32_DHT11_QtChart
    VERSION 1.0
    QML_FILES
        Main.qml README.md
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appEsp32_DHT11_QtChart PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appEsp32_DHT11_QtChart
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appEsp32_DHT11_QtChart
    PRIVATE Qt6::Quick Qt6::Charts
)

include(GNUInstallDirs)
install(TARGETS appEsp32_DHT11_QtChart
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
