cmake_minimum_required(VERSION 3.8)
project(misora2_metal_loss)

if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # add_compile_options(-Wall -Wextra -Wpedantic)
  # -Wpedantic を有効にしているため、C++17での std::make_pair の仕様変更に伴う警告が出ています。
  # これはC++14とC++17でのパラメータ渡し方法の違いによるもので、ビルドには致命的ではありません。
  # 警告を抑制するには -Wpedantic を外すか無効化してください。
  add_compile_options(-Wall -Wextra)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(OpenCV 4.8 REQUIRED COMPONENTS core highgui imgproc)

find_package(PkgConfig REQUIRED)
pkg_check_modules(TESSERACT REQUIRED tesseract)

include_directories(
  include
  ${TESSERACT_INCLUDE_DIRS}
  ${LEPTONICA_INCLUDE_DIRS}
)

set(dependencies
    rclcpp
    rclcpp_components
    cv_bridge
    OpenCV
    sensor_msgs
    std_msgs
)
# ライブラリ化
add_library(metal_loss_library SHARED src/metal_loss_component.cpp src/detect.cpp)
ament_target_dependencies(metal_loss_library ${dependencies})
target_link_libraries(metal_loss_library
  ${TESSERACT_LIBRARIES}
  ${LEPTONICA_LIBRARIES}
)
rclcpp_components_register_nodes(metal_loss_library "component_metal_loss::EvaluateMetalLoss")
# ノード
add_executable(metal_loss_node src/metal_loss_component.cpp src/metal_loss_node.cpp src/detect.cpp)
ament_target_dependencies(metal_loss_node ${dependencies})
target_link_libraries(metal_loss_node
  ${TESSERACT_LIBRARIES}
  ${LEPTONICA_LIBRARIES}
)

# C++ main関数から実行
add_executable(metal_loss_detection 
    src/detect.cpp 
    src/detect_main.cpp)

target_link_libraries(metal_loss_detection
    ${OpenCV_LIBS}
    ${TESSERACT_LIBRARIES}
    ${LEPTONICA_LIBRARIES}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(TARGETS
  metal_loss_library
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(TARGETS
  metal_loss_node
  metal_loss_detection
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
