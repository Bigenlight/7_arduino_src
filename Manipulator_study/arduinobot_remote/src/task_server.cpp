#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <rclcpp_components/register_node_macro.hpp>
#include "arduinobot_msgs/action/arduinobot_task.hpp"
#include <moveit/move_group_interface/move_group_interface.h>

#include <memory>


using namespace std::placeholders;

namespace arduinobot_remote
{
class TaskServer : public rclcpp::Node
{
public:
  explicit TaskServer(const rclcpp::NodeOptions& options = rclcpp::NodeOptions())
    : Node("task_server", options)
  {
    RCLCPP_INFO(get_logger(), "Starting the Server");
    action_server_ = rclcpp_action::create_server<arduinobot_msgs::action::ArduinobotTask>(
        this, "task_server", std::bind(&TaskServer::goalCallback, this, _1, _2),
        std::bind(&TaskServer::cancelCallback, this, _1),
        std::bind(&TaskServer::acceptedCallback, this, _1));
  }

private:
  rclcpp_action::Server<arduinobot_msgs::action::ArduinobotTask>::SharedPtr action_server_;
  // std::shared_ptr<moveit::planning_interface::MoveGroupInterface> arm_move_group_, gripper_move_group_;
  // std::vector<double> arm_joint_goal_, gripper_joint_goal_;
  std::shared_ptr<moveit::planning_interface::MoveGroupInterface> arm_move_group_;
  std::vector<double> arm_joint_goal_;

  rclcpp_action::GoalResponse goalCallback(
      const rclcpp_action::GoalUUID& uuid,
      std::shared_ptr<const arduinobot_msgs::action::ArduinobotTask::Goal> goal)
  {
    RCLCPP_INFO(get_logger(), "Received goal request with id %d", goal->task_number);
    (void)uuid;
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  rclcpp_action::CancelResponse cancelCallback(
      const std::shared_ptr<rclcpp_action::ServerGoalHandle<arduinobot_msgs::action::ArduinobotTask>> goal_handle)
  {
    (void)goal_handle;
    RCLCPP_INFO(get_logger(), "Received request to cancel goal");
    if(arm_move_group_){
      arm_move_group_->stop();
    }
    // if(gripper_move_group_){
    //   gripper_move_group_->stop();
    // }
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  void acceptedCallback(
      const std::shared_ptr<rclcpp_action::ServerGoalHandle<arduinobot_msgs::action::ArduinobotTask>> goal_handle)
  {
    // this needs to return quickly to avoid blocking the executor, so spin up a new thread
    std::thread{ std::bind(&TaskServer::execute, this, _1), goal_handle }.detach();
  }

  void execute(const std::shared_ptr<rclcpp_action::ServerGoalHandle<arduinobot_msgs::action::ArduinobotTask>> goal_handle)
  {
    RCLCPP_INFO(get_logger(), "Executing goal");
    auto result = std::make_shared<arduinobot_msgs::action::ArduinobotTask::Result>();

    // MoveIt 2 Interface
    if(!arm_move_group_){
      arm_move_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(shared_from_this(), "arm");
    }
    // if(!gripper_move_group_){
    //   gripper_move_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(shared_from_this(), "gripper");
    // }


// 선착장
    if (goal_handle->get_goal()->task_number == 0)
    {
            // Initialize arm_joint_goal_ with the specified positions
      arm_joint_goal_ = {
          0.0,      // joint_1
          -0.38722253047037647,       // joint_2
          -2.1691167910005085,        // joint_3
          -0.34212209414448047,       // joint_4
          1.3104448420473949,          // joint_5
          -9.639613808243297e-05       // joint_6
      };

      // gripper_joint_goal_ = {-0.7, 0.7};
    }
    else if (goal_handle->get_goal()->task_number == 1)
    {
      arm_joint_goal_ = {0.2, 0.2, 0.0, 0.0, 0.0, 0.0};
      // gripper_joint_goal_ = {0.0, 0.0};
    }
    else if (goal_handle->get_goal()->task_number == 2)
    {
      arm_joint_goal_ = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    }
    else if (goal_handle->get_goal()->task_number == 3)
    {
      arm_joint_goal_ = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    }
    else if (goal_handle->get_goal()->task_number == 4)
    {
      arm_joint_goal_ = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    }
    else if (goal_handle->get_goal()->task_number == 5)
    {
      arm_joint_goal_ = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    }
    else if (goal_handle->get_goal()->task_number == 6)
    {
      arm_joint_goal_ = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    }
    else if (goal_handle->get_goal()->task_number == 7)
    {
      arm_joint_goal_ = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    }
    else if (goal_handle->get_goal()->task_number == 8)
    {
      arm_joint_goal_ = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    }
    else if (goal_handle->get_goal()->task_number == 9)
    {
      arm_joint_goal_ = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    }

    //대기 자세
    else if (goal_handle->get_goal()->task_number == 10)
    {
      arm_joint_goal_ = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    }
    // 선착장에서 뒤로 뺀 상태
    else if (goal_handle->get_goal()->task_number == 11)
    {
      arm_joint_goal_ = {
        0.0,      // joint_1
        -0.38522253047037647,       // joint_2
        -2.171167910005085,        // joint_3
        -0.34212209414448047,       // joint_4
        1.3204448420473949,          // joint_5
        0.0      // joint_6
        };
    }
    
    else
    {
      RCLCPP_ERROR(get_logger(), "Invalid Task Number");
      return;
    }

    arm_move_group_->setStartState(*arm_move_group_->getCurrentState());
    // gripper_move_group_->setStartState(*gripper_move_group_->getCurrentState());

    bool arm_within_bounds = arm_move_group_->setJointValueTarget(arm_joint_goal_);
    // bool gripper_within_bounds = gripper_move_group_->setJointValueTarget(gripper_joint_goal_);
    // if (!arm_within_bounds | !gripper_within_bounds)
    // {
    //   RCLCPP_WARN(get_logger(),
    //               "Target joint position(s) were outside of limits, but we will plan and clamp to the limits ");
    //   return;
    // }
    if (!arm_within_bounds)
    {
      RCLCPP_WARN(get_logger(),
                  "Target joint position(s) were outside of limits, but we will plan and clamp to the limits ");
      return;
    }

    moveit::planning_interface::MoveGroupInterface::Plan arm_plan;
    moveit::planning_interface::MoveGroupInterface::Plan gripper_plan;
    bool arm_plan_success = (arm_move_group_->plan(arm_plan) == moveit::core::MoveItErrorCode::SUCCESS);
    // bool gripper_plan_success = (gripper_move_group_->plan(gripper_plan) == moveit::core::MoveItErrorCode::SUCCESS);
    
    // if(arm_plan_success && gripper_plan_success)
    // {
    //   RCLCPP_INFO(get_logger(), "Planner SUCCEED, moving the arme and the gripper");
    //   arm_move_group_->move();
    //   gripper_move_group_->move();
    // }
    if(arm_plan_success)
    {
      RCLCPP_INFO(get_logger(), "Planner SUCCEED, moving the arm");
      arm_move_group_->move();
      // gripper_move_group_->move();
    }
    else
    {
      RCLCPP_ERROR(get_logger(), "One or more planners failed!");
      return;
    }
  
    result->success = true;
    goal_handle->succeed(result);
    RCLCPP_INFO(get_logger(), "Goal succeeded");
  }
};
}  // namespace arduinobot_remote

RCLCPP_COMPONENTS_REGISTER_NODE(arduinobot_remote::TaskServer)