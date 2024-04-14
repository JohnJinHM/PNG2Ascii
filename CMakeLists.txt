cmake_minimum_required( VERSION 3.16 ) # pretty old, but w/e

project(Ascii)

add_executable(Demo src/main.cpp)

# find_package(PNG REQUIRED)
# include_directories("C:\\msys64\\mingw64\\include")
# target_link_libraries(Demo PNG)

# target_compile_features( Demo PUBLIC cxx_std_20 )