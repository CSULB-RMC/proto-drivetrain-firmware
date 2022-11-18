#include <Arduino.h>
#include <Servo.h>
#include <vector>
#include "main.h"

Servo drivetrainL;
Servo drivetrainR;

rcl_subscription_t motor_control_sub;
std_msgs__msg__UInt8MultiArray motor_control_msg;

rcl_node_t node;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;

#define MICROROS_MAX_SUBSCRIBERS 1

void motor_control_callback(const void * msgin) {
  const std_msgs__msg__UInt8MultiArray * msg = (const std_msgs__msg__UInt8MultiArray *)msgin;
  for(std::vector<int>::const_iterator it = msg->data.begin(); it != msg->data.end(); ++it)
	{
		msg[i] = *it;
		i++;
	}
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  set_microros_serial_transports(Serial);
  delay(2000);

  allocator = rcl_get_default_allocator();
  //printFuncln("Got default allocator");

  //create init_options
  rcl_ret_t temp_rc = rclc_support_init(&support, 0, NULL, &allocator);
 // printFuncln("rclc_support_init Success.");


  // create node
  temp_rc = rclc_node_init_default(&node, "micro_ros_arduino_node", "", &support);
  //printFuncln("rclc_node_init_default Success.");

  // create executor
  temp_rc = rclc_executor_init(&executor, &support.context, MICROROS_MAX_SUBSCRIBERS, &allocator);
  //printFuncln("rclc_executor_init Success.");

  pinMode(LED_BUILTIN, OUTPUT);
  drivetrainL.attach(33);
  drivetrainR.attach(1);

  drivetrainL.write(90);
  drivetrainR.write(90);

  rclc_subscription_init_default(&motor_control_sub, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt8MultiArray), "motor_control");

  rclc_executor_add_subscription(&executor, &motor_control_sub, &motor_control_msg, &motor_control_callback, ON_NEW_DATA);
}

void loop() {
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
  // put your main code here, to run repeatedly:
}