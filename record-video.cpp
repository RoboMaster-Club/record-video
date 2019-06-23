#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>

int main() {
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

    cv::VideoWriter writer("out.mp4", 0x00000021, 60, cv::Size(848, 480));

    while(true) {
        cv::Mat img = cv::Mat(cv::Size(image_width, image_height), CV_8UC3, (void *) color.get_data(), cv::Mat::AUTO_STEP);
        imshow("image", img);
        writer.write(img);
        cv::waitKey(1);
        char c = (char) cv::waitKey(1);
        if (c == 27)
            break;
    }
    pipe.stop();
    cfg.disable_all_streams();
}