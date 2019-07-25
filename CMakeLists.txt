cmake_minimum_required(VERSION 3.14)
project(HttpClient_beast)

set(CMAKE_CXX_STANDARD 14)


find_package(OpenSSL)
add_executable(test main_beast HttpClient)
target_include_directories(test PRIVATE
        /opt/data/libraries/boost_1_70_0/)
target_link_libraries(test
        pthread
        OpenSSL::SSL
        OpenSSL::Crypto)