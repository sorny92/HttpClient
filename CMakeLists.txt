cmake_minimum_required(VERSION 3.10)
project(HttpClient_beast)

set(CMAKE_CXX_STANDARD 14)


find_package(OpenSSL)

# Download boost 1.7
if (NOT EXISTS ${CMAKE_BINARY_DIR}/boost_1_70_0)
    file(DOWNLOAD
            https://dl.bintray.com/boostorg/release/1.70.0/source/boost_1_70_0.zip
            ${CMAKE_BINARY_DIR}/boost_1_70_0.zip
            SHOW_PROGRESS)
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar "xvf" ${CMAKE_BINARY_DIR}/boost_1_70_0.zip)
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/boost_1_70_0.zip)
endif ()

add_library(HttpClient HttpClient)
target_include_directories(HttpClient PRIVATE
        ${CMAKE_BINARY_DIR}/boost_1_70_0)
target_link_libraries(HttpClient
        pthread
        OpenSSL::SSL
        OpenSSL::Crypto)

add_executable(test_httpclient main_beast)
target_include_directories(test_httpclient PRIVATE
        ${CMAKE_BINARY_DIR}/boost_1_70_0)
target_link_libraries(test_httpclient HttpClient)