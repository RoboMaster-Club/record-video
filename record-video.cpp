#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>

int main(int argc, char** argv) {
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_COLOR, 848, 480, RS2_FORMAT_BGR8, 60);
    // Start streaming with default recommended configuration
    rs2::pipeline pipe;
    pipe.start(cfg);
    rs2::frameset data;
    data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
    rs2::video_frame color = data.get_color_frame();
    int image_width = color.get_width();
    int image_height = color.get_height();
    cv::VideoWriter writer(argv[1], CV_FOURCC('m', 'p', '4', 'v'), 60, cv::Size(848, 480));
    cv::Mat img = cv::Mat(cv::Size(image_width, image_height), CV_8UC3, (void *) color.get_data(), cv::Mat::AUTO_STEP);
    while(true) {
        imshow("image", img);
        writer.write(img);
        cv::waitKey(1);
        char c = (char) cv::waitKey(1);
        if (c == 27)
            break;
        data = pipe.wait_for_frames();
        color = data.get_color_frame();
        img = cv::Mat(cv::Size(image_width, image_height), CV_8UC3, (void *) color.get_data(), cv::Mat::AUTO_STEP);
    }
    pipe.stop();
    cfg.disable_all_streams();
}
