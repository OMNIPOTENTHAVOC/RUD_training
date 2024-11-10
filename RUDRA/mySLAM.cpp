#include <opencv2/opencv.hpp>
#include <System.h> // Include ORB-SLAM2 system header
#include <pcl/point_cloud.h>
#include <pcl/io/pcd_io.h>
#include <iostream>
#include <sensor/IMU.h> // Assume a library for your IMU
#include <vector>
#include <Eigen/Dense>


void initializeSLAM(ORB_SLAM2::System &slam);
void captureAndProcessFrames(ORB_SLAM2::System &slam, cv::VideoCapture &cap);

int main() {
    // Initialize webcam
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    // Initialize IMU
    IMU imu;

    // Initialize SLAM system
    ORB_SLAM2::System slam("path/to/vocabulary/file", "path/to/config.yaml", ORB_SLAM2::System::MONOCULAR, true);

    // Start capturing and processing frames
    captureAndProcessFrames(slam, cap);

    // Generate point cloud after SLAM processing
    generatePointCloud(slam);

    // Define start and goal points for RRT*
    Eigen::Vector2f start(0.0f, 0.0f);
    Eigen::Vector2f goal(5.0f, 5.0f); // Adjust these based on your environment
    std::vector<Eigen::Vector2f> obstacles; // Define your obstacles here

    // Run RRT* path planning
    auto path = rrtStar(start, goal, obstacles);

    // Visualize the planned path
    visualizePath(path);

    // Cleanup
    slam.Shutdown();
    cap.release();
    cv::destroyAllWindows();
    
    return 0;
}

// Function definitions
void initializeSLAM(ORB_SLAM2::System &slam) {
    // Initialization code (if needed)
}

void captureAndProcessFrames(ORB_SLAM2::System &slam, cv::VideoCapture &cap) {
    while (true) {
        cv::Mat frame;
        cap >> frame; // Capture a frame

        // Read IMU data
        float roll, pitch, yaw;
        imu.getOrientation(roll, pitch, yaw);

        // Pass the frame to the SLAM system
        slam.TrackMonocular(frame, static_cast<double>(cv::getTickCount()));

        // Display the frame
        cv::imshow("Frame", frame);
        if (cv::waitKey(30) >= 0) break; // Break on any key press
    }
}

void generatePointCloud(ORB_SLAM2::System &slam) {
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    slam.getPointCloud(cloud);
    pcl::io::savePCDFile("output.pcd", *cloud);
}
