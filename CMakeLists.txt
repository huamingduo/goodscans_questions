cmake_minimum_required(VERSION 3.0)
project(test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(compute_height
  src/1_compute_height.cc
)
add_executable(random_shuffle
  src/2_random_shuffle.cc
)
add_executable(check
  src/3_check.cc
)
add_executable(find_rectangle
  src/4_find_rectangle.cc
)
target_link_libraries(find_rectangle PRIVATE
  ${OpenCV_LIBRARIES}
)
add_executable(associate
  src/5_associate.cc
)
target_link_libraries(associate PRIVATE
  ${OpenCV_LIBRARIES}
)

set_target_properties(compute_height check random_shuffle find_rectangle associate
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)
