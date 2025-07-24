# misora2_metal_loss
## 内容
 - 受け取った画像から機器が示している数値を取得
 - 検出終了の合図(黒画像:CV_8UC1)を受け取るまで、取得した数値を保存
 - 終了時、保存している数値の中で最小の値を返す

## 実行前に
 - 文字読み取り専用ライブラリTesseractをインストールしなければならない
~~~bash!
sudo apt install tesseract-ocr #OCR本体
sudo apt install libtesseract-dev #開発用パッケージ ヘッダ、cmake用
sudo apt update
sudo apt upgarde
~~~

## 実行コード
### ノード単体で実行
~~~bash!
colcon build 
source install/setup.bash
ros2 run misora2_metal_loss metal_loss_node
~~~

### C++プログラム実行
クロップ範囲は直接プログラム(src/detect_main.cpp)に書き込む
~~~bash!
colcon build --symlink-install
source install/setup.bash
./build/misora2_metal_loss/metal_loss_detection <画像パス>
~~~