#include "misora2_metal_loss/metal_loss_component.hpp"

namespace component_metal_loss
{
EvaluateMetalLoss::EvaluateMetalLoss(const rclcpp::NodeOptions &options)
    : Node("evaluate_metal_loss", options)
{
    // 数値を読み取る領域をパラメータで設定
    this->declare_parameter<int>("x",20);
    this->declare_parameter<int>("y",20);
    this->declare_parameter<int>("width",100);
    this->declare_parameter<int>("height",100);

    int x,y,width,height;
    this->get_parameter("x",x);
    this->get_parameter("y",y);
    this->get_parameter("width",width);
    this->get_parameter("heght",height);
    // 始点(左隅)からwidth x heightの長方形領域を作成
    sp = cv::Point(x,y);
    size = cv::Size(width,height);

    // 分配機から画像を受け取る
    receive_image_ = this->create_subscription<MyAdaptedType>("metal_loss_image",10,std::bind(&EvaluateMetalLoss::update_image_callback,this,std::placeholders::_1));
    
    // 数値の報告
    minimal_value_publisher_ = this->create_publisher<std_msgs::msg::String>("metal_loss_result_data",10);
    result_image_publisher_ = this->create_publisher<sensor_msgs::msg::Image>("metal_loss_result_image",10);//不要だったらコメントアウト
}   

void EvaluateMetalLoss::update_image_callback(const std::unique_ptr<cv::Mat> msg){
    receive_image = std::move(*msg);

    RCLCPP_INFO_STREAM(this->get_logger(),"Receive image address: " << &(msg->data));
    if (cv::countNonZero(image) == 0) {
        // 黒画像が来たら保存している最小値を報告する
        // (1) int minimal_value = *min_element(begin(value_list), end(value_list));
        // (2)はそのままminimal_valueを送信
        result_value.data = std::to_string(minimal_value);
        minimal_value_publisher_->publish(result_value);
    }
    else {
        // cv::Mat型のreceive_imageと数値を読み取る領域(sp,size)を入力とした数値読み取り関数 返り値 double型
        // double result = func(receive_image,sp,size);
        // (1)value_list.push_back(result);
        // (2)if(minimal_value < result) minimal_value = result;
    }
}

} //namespace component_metal_loss
#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(component_metal_loss::EvaluateMetalLoss)