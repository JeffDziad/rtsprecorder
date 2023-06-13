#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace cv;

const int SHOW_WIDTH = 640;
const int SHOW_HEIGHT = 360;

void camcorder(std::string url) {
    std::chrono::steady_clock::time_point startPoint = std::chrono::steady_clock::now();
    int frameCount = 0;
    VideoCapture cap(url, CAP_FFMPEG);
    Mat frame;
    Mat showFrame;
    if (!cap.isOpened()) {
        std::cout << "Cannot open RTSP stream" << std::endl;
    } else {
        while (true) {
            cap >> frame;
            frameCount++;

            resize(frame, showFrame, Size(SHOW_WIDTH, SHOW_HEIGHT), INTER_LINEAR);
            Point fpsPosition(10, 30);
            int fpsFontSize = 1;
            Scalar fpsColor(0, 255, 0);
            int fpsWeight = 2;

            std::chrono::steady_clock::time_point elapsedPoint = std::chrono::steady_clock::now();
            int fps = frameCount / std::chrono::duration_cast<std::chrono::seconds>(elapsedPoint - startPoint).count();

            putText(showFrame, "FPS: " + std::to_string(fps), fpsPosition, FONT_HERSHEY_COMPLEX, fpsFontSize, fpsColor, fpsWeight);
            imshow(url, showFrame);


            if (waitKey(1) == 27) {
                break;
            }
        }
    }

    cap.release();
    destroyAllWindows();
}

int main()
{
    const std::string RTSP_URL = "rtsp://beverly1:0FtYard1@192.168.1.245/live";

#if WIN32
    _putenv_s("OPENCV_FFMPEG_CAPTURE_OPTIONS", "rtsp_transport;udp");
#else
    setenv("OPENCV_FFMPEG_CAPTURE_OPTIONS", "rtsp_transport;udp", 1);
#endif

    std::thread cam1(camcorder, "rtsp://beverly1:0FtYard1@192.168.1.245/live");

    cam1.join();

    return 0;
}