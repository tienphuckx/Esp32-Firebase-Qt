## Qt Charts


## 1. Install Vulkan, Set up environments
    https://vulkan.lunarg.com/sdk/home
    echo %VULKAN_SDK%


## 2. Config in Cmake

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
    

    find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Charts)
    
    target_link_libraries(appEsp32_DHT11_QtChart
        PRIVATE Qt6::Quick Qt6::Charts
    )
    

## 3. you have to replace GuiApplication with QApplication, the reason for this is your use of QCharts

    #include <QApplication>
    #include <QQmlApplicationEngine>
    
    int main(int argc, char *argv[])
    {
        QApplication app(argc, argv);
        
