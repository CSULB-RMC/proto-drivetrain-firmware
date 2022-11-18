#include <Arduino.h>
#include <Servo.h>
#include "main.h"

Servo drivetrainL;
Servo drivetrainR;

rcl_subscription_t motor_control_sub;
//std_msgs__msg__UInt8MultiArray motor_control_msg;
std_msgs__msg__UInt16 motor_control_msg;

rcl_node_t node;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;

#define MICROROS_MAX_SUBSCRIBERS 4

void motor_control_callback(const void * msgin) {
  const std_msgs__msg__UInt16 * msg = (const std_msgs__msg__UInt16 *)msgin;
  int left, right;
  left = 0xFF & (msg->data);
  right = (msg->data) >> 8;
  //uint8_t *dtl = (uint8_t * )msg;
  digitalToggle(LED_BUILTIN);
  if(left > 170) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  drivetrainL.write(left);
  drivetrainR.write(180-right);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600);
  Serial2.println("test");
  set_microros_serial_transports(Serial);
  delay(2000);

  allocator = rcl_get_default_allocator();
  //printFuncln("Got default allocator");

  //create init_options

  RCSOFTCHECK(rclc_support_init(&support, 0, NULL, &allocator));
 // printFuncln("rclc_support_init Success.");


  // create node
  RCSOFTCHECK(rclc_node_init_default(&node, "micro_ros_arduino_node", "", &support));
  //printFuncln("rclc_node_init_default Success.");

  // create executor
  RCSOFTCHECK(rclc_executor_init(&executor, &support.context, MICROROS_MAX_SUBSCRIBERS, &allocator));

  pinMode(LED_BUILTIN, OUTPUT);
  drivetrainL.attach(33);
  drivetrainR.attach(1);

  drivetrainL.write(90);
  drivetrainR.write(90);

  RCSOFTCHECK(rclc_subscription_init_default(&motor_control_sub, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt16), "motor_control"));

  RCSOFTCHECK(rclc_executor_add_subscription(&executor, &motor_control_sub, &motor_control_msg, &motor_control_callback, ON_NEW_DATA));
  
  Serial2.println("Done with setup.");
  delay(2000);
  //digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
  // put your main code here, to run repeatedly:
}