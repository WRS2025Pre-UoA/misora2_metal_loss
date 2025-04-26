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
#include <cv_bridge/cv_bridge.h>
#include <rclcpp/type_adapter.hpp>

#include "misora2_metal_loss/cv_mat_type_adapter.hpp"

using namespace std::chrono_literals;

namespace component_metal_loss
{
class EvaluateMetalLoss : public rclcpp::Node
{
public:
    using MyAdaptedType = rclcpp::TypeAdapter<cv::Mat, sensor_msgs::msg::Image>;

    std_msgs::msg::String result_value;
    cv::Mat result_image, receive_image;

    // 最小値の判断
    std::vector<double> value_list; // (1)関数から返ってきた最小値を格納する配列
    double minimal_value = std::numeric_limits<double>::max(); // (2)関数から返ってきた値が現在の値より小さいか判別

    cv::Point point;
    cv::Size size;
    explicit EvaluateMetalLoss(const rclcpp::NodeOptions &options);
    EvaluateMetalLoss() : EvaluateMetalLoss(rclcpp::NodeOptions{}) {}

private:
    void update_image_callback(const std::unique_ptr<cv::Mat> msg);
    // double func(cv::Mat img, cv::Point sp, cv::Size size);
    rclcpp::Subscription<MyAdaptedType>::SharedPtr receive_image_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr minimal_value_publisher_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr result_image_publisher_;
};

} // namespace component_metal_loss

#endif // METAL_LOSS_COMPONENT_HPP