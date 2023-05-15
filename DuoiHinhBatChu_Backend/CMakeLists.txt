cmake_minimum_required(VERSION 3.14)

project(DuoiHinhBatChu_Backend LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core HttpServer Sql Concurrent)

add_executable(DuoiHinhBatChu_Backend
  main.cpp
  auth.cpp
  auth.h
  room.cpp
  room.h
  user.cpp
  user.h
  question.cpp
  question.h
  protocolSocket.h
  connectionPool.h
  database.h
  database.cpp
  history.h
  history.cpp
  rank.h
  rank.cpp
)
target_link_libraries(DuoiHinhBatChu_Backend Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::HttpServer Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Concurrent)

install(TARGETS DuoiHinhBatChu_Backend
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
