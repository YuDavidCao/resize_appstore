#include <filesystem>
#include <iostream>
#include <cstring>
#include <opencv2/opencv.hpp>
namespace fs = std::filesystem;

const int IPHONE_WIDTH = 1290;
const int IPHONE_HEIGHT = 2796;
const int IPAD_WIDTH = 2048;
const int IPAD_HEIGHT = 2732;

std::string getResizedFileName(const std::string &path) {
    size_t lastSlashIndex = path.find_last_of("/");
    return path.substr(0, lastSlashIndex + 1) + "resized_" + path.substr(lastSlashIndex + 1);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <device_type>" << std::endl;
        std::cerr << "device_type: 'iphone' / 'mobile' or 'ipad' / 'tablet'" << std::endl;
        return 1;
    }
    std::string mode;
    if(strcmp(argv[1], "iphone") == 0 || strcmp(argv[1], "mobile") == 0) {
        mode = "mobile";
    } else if(strcmp(argv[1], "ipad") == 0 || strcmp(argv[1], "tablet") == 0) {
        mode = "tablet";
    } else {
        std::cerr << "Usage: " << argv[0] << " <device_type>" << std::endl;
        std::cerr << "device_type: 'iphone' / 'mobile' or 'ipad' / 'tablet'" << std::endl;
        return 1;
    }
    fs::path currentDirectory = fs::current_path();
    for (const auto &entry : fs::directory_iterator(currentDirectory.string())) {
        std::string extension = entry.path().extension().string();
        std::cout << extension << std::endl;
        if (extension != ".png" && extension != ".jpg" && extension != ".jpeg" && extension != ".PNG" && extension != ".JPG" && extension != ".JPEG") {
            continue;
        }
        cv::Mat inputImage = cv::imread(entry.path().string(), cv::IMREAD_UNCHANGED);
        if (inputImage.empty()) {
            std::cerr << "Error: Could not open or find the image: " << entry.path().string() << std::endl;
        }
        // std::cout << "Original Image Size: " << inputImage.cols << "x" << inputImage.rows << std::endl;
        cv::Mat resizedImage;
        int targetWidth, targetHeight;
        if(mode == "mobile") {
            cv::resize(inputImage, resizedImage, cv::Size(IPHONE_WIDTH, IPHONE_HEIGHT), 0, 0, cv::INTER_AREA);
        } else if(mode == "tablet") {
            cv::resize(inputImage, resizedImage, cv::Size(IPAD_WIDTH, IPAD_HEIGHT), 0, 0, cv::INTER_AREA);
        } else {
            std::cerr << "Error: Invalid device type. Please use 'iphone' or 'ipad'." << std::endl;
            return 1;
        }
        // std::cout << "Resized Image Size: " << resizedImage.cols << "x" << resizedImage.rows << std::endl;
        std::string outputImagePath = getResizedFileName(entry.path().string());
        if (!cv::imwrite(outputImagePath, resizedImage)) {
            std::cerr << "Error: Could not save the resized image:" << outputImagePath << std::endl;
        } else {
            std::cout << mode << ": Resized image saved as " << outputImagePath << std::endl;
        }
    }
    return 0;
}
