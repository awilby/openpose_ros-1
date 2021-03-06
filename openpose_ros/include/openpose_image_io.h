#ifndef _OPENPOSE_IMAGE_IO
#define _OPENPOSE_IMAGE_IO

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <std_msgs/Header.h>

#include <opencv2/core/core.hpp>

#include <openpose_ros_msgs/BoundingBox.h>
#include <openpose_ros_msgs/OpenPoseHuman.h>
#include <openpose_ros_msgs/OpenPoseHumanList.h>
#include <openpose_ros_msgs/PointWithProb.h>
#include <openpose_ros_msgs/HandKeypoints.h>
#include <openpose_ros_msgs/FaceFeatures.h>
#include <openpose_ros_msgs/BodyFeatures.h>
#include <openpose_ros_msgs/BodyPart.h>

#include <openpose.h>
#include <gflags_options.h>

// OpenPose dependencies
#include <openpose/headers.hpp>

namespace openpose_ros {

    class OpenPoseImageIO
    {
        private:
            ros::NodeHandle nh_;
            ros::Publisher openpose_human_list_pub_;
            image_transport::ImageTransport it_;
            image_transport::Subscriber image_sub_;
            cv_bridge::CvImagePtr cv_img_ptr_;
            std_msgs::Header rgb_image_header_;

            OpenPose* openpose_;

        public:
            OpenPoseImageIO(OpenPose &openPose);

            ~OpenPoseImageIO(){}

            void processImage(const sensor_msgs::ImageConstPtr& msg);

            void convertImage(const sensor_msgs::ImageConstPtr& msg);

            std::shared_ptr<std::vector<op::Datum>> createDatum();

            bool display(const std::shared_ptr<std::vector<op::Datum>>& datumsPtr,
                         const openpose_ros_msgs::OpenPoseHumanList);

            cv_bridge::CvImagePtr& getCvImagePtr();

            openpose_ros_msgs::OpenPoseHumanList processFrame(const std::shared_ptr<std::vector<op::Datum>>& datumsPtr);

            template <typename T> openpose_ros_msgs::OpenPoseHuman extractBodyPartKeypoints(
                                                T poseKeypoints,
                                                openpose_ros_msgs::OpenPoseHuman human,
                                                int person_num);

            template <typename T> openpose_ros_msgs::OpenPoseHuman extractFaceKeypoints(
                                                T faceKeypoints,
                                                std::vector<op::Rectangle<float>>& face_rectangles,
                                                openpose_ros_msgs::OpenPoseHuman human,
                                                int person_num);

            template <typename T> openpose_ros_msgs::OpenPoseHuman extractHandKeypoints(
                                                T leftHandKeypoints, T rightHandKeypoints,
                                                std::vector<std::array<op::Rectangle<float>, 2>>& hand_rectangles,
                                                openpose_ros_msgs::OpenPoseHuman human,
                                                int person_num);

            openpose_ros_msgs::OpenPoseHumanList getHeatMaps(const std::shared_ptr<std::vector<op::Datum>>& datumsPtr,
                                                        openpose_ros_msgs::OpenPoseHumanList human_list);

            void publishKeypoints(const openpose_ros_msgs::OpenPoseHumanList);

            template <typename T> void printKeypoints(T poseKeypoints, T faceKeypoints,
                                              T leftHandKeypoints, T rightHandKeypoints);

            template <typename T> void printHeatmaps(T poseHeatMaps, T faceHeatMaps,
                                              T leftHandHeatMaps, T rightHandHeatMaps);

    };
}

#endif
