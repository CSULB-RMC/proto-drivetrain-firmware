#ifndef LBL_MAIN_H
#define LBL_MAIN_H



#include <micro_ros_platformio.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

//#include <std_msgs/msg/u_int8_multi_array.h>
#include <std_msgs/msg/u_int8.h>

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){Serial2.print(#fn " Error Code: "); Serial2.println(temp_rc); return -1;}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){Serial2.print(#fn " Error Code: "); Serial2.println(temp_rc);}}
#define RCCLASSCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printFunc(#fn " Error Code: "); printFuncln(temp_rc); microros_error = true;}}


#endif // LBL_MAIN_H