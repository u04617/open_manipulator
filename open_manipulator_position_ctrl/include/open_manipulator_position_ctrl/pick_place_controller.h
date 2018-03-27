/*******************************************************************************
* Copyright 2016 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/* Authors: Taehun Lim (Darby) */

#ifndef OPEN_MANIPULATOR_PICK_AND_PLACE_CONTROLLER_H
#define OPEN_MANIPULATOR_PICK_AND_PLACE_CONTROLLER_H

#include <ros/ros.h>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/planning_interface/planning_interface.h>

#include <moveit_msgs/DisplayTrajectory.h>

#include <std_msgs/Float64.h>
#include <std_msgs/String.h>

#include <sensor_msgs/JointState.h>

#include "open_manipulator_msgs/JointPose.h"
#include "open_manipulator_msgs/KinematicsPose.h"

#include <boost/thread.hpp>

#include <eigen3/Eigen/Eigen>

namespace open_manipulator
{
#define JOINT_NUM 4
#define GRIP_NUM  1

#define LEFT_PALM   0
#define RIGHT_PALM  1

#define ITERATION_FREQUENCY 25 //Hz

typedef struct
{
  std::string name;
  uint8_t id;
  uint8_t joint_num;
} Joint;

typedef struct
{
  uint16_t waypoints;                                  // planned number of via-points
  ros::Duration time_from_start;                       // planned movement time
  Eigen::MatrixXd planned_path_positions;              // planned position trajectory
} PlannedPathInfo;

class PickAndPlaceController
{
 private:
  // ROS NodeHandle
  ros::NodeHandle nh_;

  // ROS Parameters
  bool using_gazebo_;
  std::string robot_name_;

  // ROS Publisher
  ros::Publisher gazebo_goal_joint_position_pub_[JOINT_NUM];
  ros::Publisher gazebo_gripper_position_pub_[2];

  // ROS Subscribers
  ros::Subscriber gazebo_present_joint_position_sub_;
  ros::Subscriber display_planned_path_sub_;
  ros::Subscriber target_joint_pose_sub_;
  ros::Subscriber target_kinematics_pose_sub_;

  // ROS Service Server

  // ROS Service Client

  // Joint states
  Joint joint_[4];
  Joint gripper_;

//  Eigen::VectorXd present_joint_position_;
//  Eigen::VectorXd goal_joint_position_;
  Eigen::MatrixXd goal_trajectory_;

  // MoveIt! interface
  std::string planning_group_;
  moveit::planning_interface::MoveGroupInterface *move_group;
  moveit_msgs::DisplayTrajectory trajectory_msg_;
  PlannedPathInfo planned_path_info_;

  // thread
  boost::thread* trajectory_generate_thread_;

  // Process state variables
  bool is_moving_;
  double   move_time_;
  uint16_t all_time_steps_;

 public:
  PickAndPlaceController();
  virtual ~PickAndPlaceController();

  void process(void);

 private:
  void initPublisher(bool using_gazebo);
  void initSubscriber(bool using_gazebo);

  void moveItTrajectoryGenerateThread();

  void gazeboPresentJointPositionMsgCallback(const sensor_msgs::JointState::ConstPtr &msg);
  void displayPlannedPathMsgCallback(const moveit_msgs::DisplayTrajectory::ConstPtr &msg);

  void targetJointPoseMsgCallback(const open_manipulator_msgs::JointPose::ConstPtr &msg);
  void targetKinematicsPoseMsgCallback(const open_manipulator_msgs::KinematicsPose::ConstPtr &msg);
};
}

#endif /*OPEN_MANIPULATOR_PICK_AND_PLACE_CONTROLLER_H*/
