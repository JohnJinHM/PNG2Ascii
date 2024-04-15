cmake_minimum_required( VERSION 3.16 )

project(Ascii)

add_executable(Demo src/main.cpp src/read_png.cpp)

find_package(PNG REQUIRED)
include_directories("C:\\msys64\\mingw64\\include")
target_link_libraries(Demo PNG)

target_compile_features( Demo PUBLIC cxx_std_20 )