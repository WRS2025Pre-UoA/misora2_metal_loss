#include "misora2_metal_loss/metal_loss_component.hpp"

namespace component_metal_loss
{
EvaluateMetalLoss::EvaluateMetalLoss(const rclcpp::NodeOptions &options)
    : Node("evaluate_metal_loss", options)
{
    receive_image_ = this->create_subscription<MyAdaptedType>("metal_loss_image",10,std::bind(&EvaluateMetalLoss::update_image_callback,this,std::placeholders::_1));
    
    minimal_value_publisher_ = this->create_publisher<std_msgs::msg::String>("metal_loss_result_data",10);
    result_image_publisher_ = this->create_publisher<sensor_msgs::msg::Image>("metal_loss_result_image",10);//不要だったらコメントアウト

    // cv::Mat型のreceive_imageを入力としたメーター値検出関数 返り値std::pair<string,cv::Mat>func(cv::Mat ) 画像の出力いらないかも
    // auto [minimal_value, result_image] = func(receive_image)
}   

void EvaluateMetalLoss::update_image_callback(const std::unique_ptr<cv::Mat> msg){
    receive_image = std::move(*msg);

    RCLCPP_INFO_STREAM(this->get_logger(),"Receive image address: " << &(msg->data));
    
}

} //namespace component_metal_loss
#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(component_metal_loss::EvaluateMetalLoss)