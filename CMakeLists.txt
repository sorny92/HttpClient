cmake_minimum_required(VERSION 3.14)
project(HttpClient_beast)

set(CMAKE_CXX_STANDARD 14)


find_package(OpenSSL)

add_library(HttpClient HttpClient)
target_include_directories(HttpClient PRIVATE
        /opt/data/libraries/boost_1_70_0/)
target_link_libraries(HttpClient
        pthread
        OpenSSL::SSL
        OpenSSL::Crypto)

add_executable(test main_beast)
target_include_directories(test PRIVATE
        /opt/data/libraries/boost_1_70_0/)
target_link_libraries(test HttpClient)