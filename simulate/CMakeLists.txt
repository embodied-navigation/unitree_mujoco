cmake_minimum_required(VERSION 3.16)
project(unitree_mujoco)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
  message("Debug mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -O0 -fPIC")
else()
  message("Release mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -DNDEBUG -fPIC")
endif()

list(APPEND CMAKE_PREFIX_PATH "/opt/unitree_robotics/lib/cmake")
find_package(unitree_sdk2 REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)


include_directories(
  /usr/local/include/iceoryx/v2.0.2
  mujoco/include
  mujoco/simulate
  src
  src/lodepng
)
link_directories(mujoco/lib)

file(GLOB SIM_SRC
  src/joystick/joystick.cc

  # exclude mujoco/simulate/main.cc
  mujoco/simulate/glfw_*.cc
  mujoco/simulate/platform_*.cc
  mujoco/simulate/simulate.cc
  src/lodepng/lodepng.cpp
)

link_libraries(
  pthread
  mujoco
  glfw
  yaml-cpp
  unitree_sdk2
  boost_program_options
  fmt
)

add_executable(unitree_mujoco ${SIM_SRC} src/main.cc)

add_executable(jstest src/joystick/jstest.cc src/joystick/joystick.cc)

enable_testing()
add_executable(keyboard_joystick_test test/keyboard_joystick_test.cc)
target_include_directories(keyboard_joystick_test PRIVATE src)
target_compile_options(keyboard_joystick_test PRIVATE -UNDEBUG)
add_test(NAME keyboard_joystick_test COMMAND keyboard_joystick_test)

add_executable(support_polygon_test test/support_polygon_test.cc)
target_include_directories(support_polygon_test PRIVATE src)
target_compile_options(support_polygon_test PRIVATE -UNDEBUG)
add_test(NAME support_polygon_test COMMAND support_polygon_test)

add_executable(user_scene_utils_test test/user_scene_utils_test.cc)
target_include_directories(user_scene_utils_test PRIVATE src mujoco/include)
target_compile_options(user_scene_utils_test PRIVATE -UNDEBUG)
add_test(NAME user_scene_utils_test COMMAND user_scene_utils_test)
