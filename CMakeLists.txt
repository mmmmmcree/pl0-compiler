cmake_minimum_required(VERSION 3.10)
project(PL0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS ON)  # 启用编译器扩展
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions(-DSOURCE_DIR="\${CMAKE_SOURCE_DIR}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(./include)
aux_source_directory(./src srcs) 
                          
add_executable(${PROJECT_NAME}
    ${srcs}  
)   
       
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/test/
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/test
)    
      