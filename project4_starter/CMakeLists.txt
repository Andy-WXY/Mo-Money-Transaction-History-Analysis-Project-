cmake_minimum_required(VERSION 3.23)
project(project4_starter)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(project4_starter
        History.hpp
        History_Transaction_definitions.cpp
        main.cpp
        project4.cpp
        project4.hpp
        Transaction.hpp
        transaction_history.txt)
