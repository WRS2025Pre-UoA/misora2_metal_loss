#include "misora2_metal_loss/detect.hpp"

int main(int argc, char *argv[]) {
    if(argc < 2){
        std::cout << "Please input file path" << std::endl;
        return 0;
    }
    std::string path = argv[1];
    // 画像の読み込み
    cv::Mat image = cv::imread(path);
    if (image.empty()) {
        std::cerr << "Cannot read image path" << std::endl;
        return -1;
    }

    try {
        int width = image.cols;
        int height = image.rows;
        // cv::Mat thresh = preprocessAndCrop(image, int(width/4), int(height/3), int(width-width/4), int(height/2)); // IMG_6
        // cv::Mat thresh = preprocessAndCrop(image, int(width/4), int(15), int(width/2-15), int(height/2+15)); // IMG_8
        cv::Mat thresh = Detect::preprocessAndCrop(image, int(width/4), int(height/3+15), int(width/2-15), int(height/3)); // IMG_3
        // std::cout << int(width/4)<<int(height/3+15)<< int(width/2-15) <<  int(height/3) << std::endl;
        double number = Detect::extractNumberFromImage(thresh);
        
        std::cout << "Success Result: " << number << std::endl;

        // 表示
        cv::imshow("thresh", thresh);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    catch (const std::exception& e) {
        std::cerr << "❌ 認識できた数字が見つかりませんでした（エラー: " << e.what() << ")" << std::endl;
        return -1;
    }

    return 0;
}
