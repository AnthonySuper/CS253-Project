cmake_minimum_required(VERSION 2.8.0)

project(cs253)

set(PROJECT_SRCS
  src/depth_image.cpp
  src/histogram.cpp
  src/image_dataset.cpp
  src/image_group.cpp
  src/image_grouper.cpp
  src/perception.cpp
  src/perception_trainer.cpp)


set(PROJECT_HEADERS
  include/depth_image.hpp
  include/errors.hpp
  include/histogram.hpp
  include/image_dataset.hpp
  include/image_group.hpp
  include/image_grouper.hpp
  include/perception.hpp
  include/perception_trainer.hpp)


include_directories(include/)

add_executable(PA8
  main.cpp
  ${PROJECT_SRCS}
  ${PROJECT_HEADERS})

set(CMAKE_CXX_FLAGS "-std=c++1z ${CMAKE_CXX_FLAGS}")
