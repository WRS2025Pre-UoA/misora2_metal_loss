#ifndef METAL_LOSS_COMPONENT_HPP
#define METAL_LOSS_COMPONENT_HPP

#include <iostream>
#include <string>
#include <memory>
#include <chrono>
#include <functional>
#include <algorithm>

#include <rclcpp/clock.hpp>
#include <rclcpp/time.hpp>

#include <opencv2/opencv.hpp>
#include "rclcpp/rclcpp.hpp"
#include <std_msgs/msg/string.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <rclcpp/type_adapter.hpp>

#include "misora2_metal_loss/cv_mat_type_adapter.hpp"

using namespace std::chrono_literals;

namespace component_metal_loss
{
class EvaluateMetalLoss : public rclcpp::Node
{
public:
    using MyAdaptedType = rclcpp::TypeAdapter<cv::Mat, sensor_msgs::msg::Image>;

    // 最小値の判断
    std::vector<double> value_list; // (1)関数から返ってきた最小値を格納する配列
    double minimal_value = std::numeric_limits<double>::max(); // (2)関数から返ってきた値が現在の値より小さいか判別

    int x,y,width,height; // クロップ範囲設定

    explicit EvaluateMetalLoss(const rclcpp::NodeOptions &options);
    EvaluateMetalLoss() : EvaluateMetalLoss(rclcpp::NodeOptions{}) {}

private:
    void update_image_callback(const std::unique_ptr<cv::Mat> msg);
    std::string to_string_with_precision(double value, int precision);

    rclcpp::Subscription<MyAdaptedType>::SharedPtr receive_image_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr minimal_value_publisher_;
    rclcpp::Publisher<MyAdaptedType>::SharedPtr result_image_publisher_;
};

} // namespace component_metal_loss

#endif // METAL_LOSS_COMPONENT_HPP