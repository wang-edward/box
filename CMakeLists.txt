cmake_minimum_required(VERSION 3.10)  # or higher depending on your requirements

# Name of the project
project(MyProject)

# Enable C++11/14/17 features (change to what you need)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create an executable called "hello_world" from the source file "hello_world.cc"
add_executable(hello_world
    hello_world.cc
)
