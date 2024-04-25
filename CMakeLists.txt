cmake_minimum_required(VERSION 3.10)

project(chat)

set(CLIENT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/client)
set(SERVER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/server)

set(GENERAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/general/include)
file(GLOB_RECURSE GENERAL_SOURCES general/src/*.cpp)

# Поиск и установка Boost и OpenSSL
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Client
file(GLOB_RECURSE CLIENT_SOURCES client/*.cpp)
add_executable(client ${CLIENT_SOURCES} ${GENERAL_SOURCES})
target_include_directories(client PUBLIC ${BOOST_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${CLIENT_INCLUDE_DIR} ${GENERAL_INCLUDE_DIR})
target_link_libraries(client ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES})

# Server
file(GLOB_RECURSE SERVER_SOURCES server/*.cpp)
add_executable(server ${SERVER_SOURCES} ${GENERAL_SOURCES})
target_include_directories(server PUBLIC ${BOOST_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${SERVER_INCLUDE_DIR} ${GENERAL_INCLUDE_DIR})
target_link_libraries(server ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES})
