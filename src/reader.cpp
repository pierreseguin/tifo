#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "jsteg_image.hh"

int main(int argc, char *argv[])
{
    if (argc != 2)
        throw std::invalid_argument("Expected 1 argument");

    std::string img_name = argv[1];
    int extension_pos = img_name.find_last_of(".");
    cv::Mat img;
    if (img_name.substr(extension_pos) == ".jsteg")
    {
        JstegImage jsteg_img = JstegImage(img_name, Image::COLOR_RGB);
        cv::Mat rgb_img = cv::Mat(jsteg_img.height, jsteg_img.width, CV_8UC3,
                                  jsteg_img.pixels.data());
        cv::cvtColor(rgb_img, img, cv::COLOR_RGB2BGR);
    }
    else
        img = cv::imread(img_name, cv::IMREAD_COLOR);

    cv::namedWindow(img_name, cv::WINDOW_GUI_NORMAL | cv::WINDOW_AUTOSIZE);
    cv::moveWindow(img_name, 0, 0);
    cv::imshow(img_name, img);
    cv::waitKey(0);

    cv::destroyAllWindows();
    return 0;
}
