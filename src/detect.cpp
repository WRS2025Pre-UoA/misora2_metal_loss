#include "misora2_metal_loss/detect.hpp"

// 前処理とクロップを行う関数
cv::Mat Detect::preprocessAndCrop(const cv::Mat& image, int x, int y, int width, int height) {
    // std::cout << x << " " << y << " " << width << " " << height << std::endl;
    // グレースケール
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // std::cout << gray.cols << gray.rows << std::endl;

    // ROIでクロップ
    // 画像サイズ内にROIが収まるかチェック
    int roi_x = std::max(0, x);
    int roi_y = std::max(0, y);
    int roi_width = std::min(width, gray.cols - roi_x);
    int roi_height = std::min(height, gray.rows - roi_y);

    if (roi_width <= 0 || roi_height <= 0) {
        std::cerr << "ROIの幅または高さが不正です: "
                << "width=" << roi_width << ", height=" << roi_height << std::endl;
        // 適宜エラー処理または戻る
    }

    cv::Rect roi(roi_x, roi_y, roi_width, roi_height);
    cv::Mat cropped = gray(roi);
    std::cout << cropped.cols << cropped.rows << std::endl;

    // ぼかし＋しきい値処理
    cv::Mat blurred, thresh;
    cv::GaussianBlur(cropped, blurred, cv::Size(3, 3), 0);
    cv::threshold(blurred, thresh, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

    return thresh;
}

// OCRで数字を抽出する関数
double Detect::extractNumberFromImage(const cv::Mat& thresh) {
    tesseract::TessBaseAPI tess;
    double error_value = std::numeric_limits<double>::max();
    if (tess.Init(NULL, "eng", tesseract::OEM_LSTM_ONLY)) {
        std::cerr << "Tesseract初期化に失敗しました" << std::endl;
        return std::numeric_limits<double>::max();
    }

    tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    tess.SetVariable("tessedit_char_whitelist", "0123456789.");
    tess.SetImage((uchar*)thresh.data, thresh.cols, thresh.rows, 1, thresh.step);

    std::string text = tess.GetUTF8Text();

    // 正規表現で数字を抽出
    std::regex numberRegex(R"(\d+\.\d+|\d+)");
    std::smatch match;
    if (std::regex_search(text, match, numberRegex)) {
        std::string numberStr = match.str();

        // 「00.49」などの不正フォーマットを検出
        if (std::regex_match(numberStr, std::regex(R"(0\d+\.\d+|0\d+)"))) {
            std::cerr << "不正な数値形式が検出されました: " << numberStr << std::endl;
            return error_value;
        }

        try {
            return std::stod(numberStr);
        } catch (...) {
            std::cerr << "文字列の変換に失敗しました: " << numberStr << std::endl;
            return error_value;
        }
    }

    std::cerr << "数値が認識できませんでした" << std::endl;
    return error_value;
}