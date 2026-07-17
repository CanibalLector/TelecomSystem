cmake_minimum_required(VERSION 3.16)

project(TelecomSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt6 6.5.2 REQUIRED COMPONENTS Core Network)

add_subdirectory(Server)
add_subdirectory(Client)
