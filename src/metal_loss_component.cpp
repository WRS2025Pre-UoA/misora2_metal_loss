#include "misora2_metal_loss/metal_loss_component.hpp"
#include "misora2_metal_loss/detect.hpp"
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

   
    this->get_parameter("x",x);
    this->get_parameter("y",y);
    this->get_parameter("width",width);
    this->get_parameter("height",height);

    RCLCPP_INFO_STREAM(this->get_logger(),"Set param(Crop Area): " << x << " " << y << " " << width << " " << height);
    // 分配機から画像を受け取る
    receive_image_ = this->create_subscription<MyAdaptedType>("metal_loss_image",10,std::bind(&EvaluateMetalLoss::update_image_callback,this,std::placeholders::_1));
    
    // 数値の報告
    minimal_value_publisher_ = this->create_publisher<std_msgs::msg::String>("metal_loss_result_data",10);
    result_image_publisher_ = this->create_publisher<MyAdaptedType>("metal_loss_result_image",10);//不要だったらコメントアウト
}   

void EvaluateMetalLoss::update_image_callback(const std::unique_ptr<cv::Mat> msg){
    cv::Mat receive_image = std::move(*msg);

    if (not(receive_image.empty())){
        if (receive_image.channels() != 1){// カラー画像である
            // 実装分部
            cv::Mat thresh = Detect::preprocessAndCrop(receive_image,x,y,width,height); // 画像、左上(x,y)、サイズ(w,h)
            double value = Detect::extractNumberFromImage(thresh);
        
            // value_list.push_back(value); // 配列に一時保存
            // if( minimal_value > value ) minimal_value = value; // 最小値をどんどん更新
            RCLCPP_INFO_STREAM(this->get_logger(), "Success read value: " << value);
            // ---------------------------------------------------
        }
        else if(receive_image.channels() == 1){// 黒画像の場合
            // std_msgs::msg::String msg_S;
            // double min_value = *min_element(begin(value_list), end(value_list));
            // msg_S.data = to_string_with_precision(minimal_value,6);
            // msg_S.data = to_string_with_precision(min_value,6);
            // minimal_value_publisher_->publish(msg_S);
            // result_image_publisher_->publish(receive_image);
            // value_list.clear(); // 送信後初期化
            // minimal_value = std::numeric_limits<double>::max(); // 送信後初期化
            // テスト用-------------------------------------------
            std_msgs::msg::String msg_S;
            msg_S.data = "0.04";
            minimal_value_publisher_->publish(msg_S);
            result_image_publisher_->publish(receive_image);
            RCLCPP_INFO_STREAM(this->get_logger(),"Publish: "<< receive_image.size() );
            // ---------------------------------------------------
        } 
    }
}

std::string EvaluateMetalLoss::to_string_with_precision(double value, int precision = 6) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;
    return out.str();
}

} //namespace component_metal_loss
#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(component_metal_loss::EvaluateMetalLoss)