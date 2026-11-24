#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>

#if !defined(MICRO_ROS_TRANSPORT_ARDUINO_SERIAL)
#error This example is only avaliable for Arduino framework with serial transport.
#endif

GY521 sensor(0x68);

float angle_x = 0.0;
float angle_y = 0.0;

SemaphoreHandle_t imu_mutex;

TaskHandle_t imu_task_handle;

rcl_publisher_t publisher;
std_msgs__msg__Int32 msg;

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

// Error handle loop
void error_loop() {
  while(1) {
    delay(100);
  }
}

void task_gy521(void *pvParameters)
{
    while (true)
    {
        sensor.read();

        float x = sensor.getAngleX();
        float y = sensor.getAngleY();

        if (xSemaphoreTake(imu_mutex, pdMS_TO_TICKS(10)) == pdTRUE)
        {
            angle_x = x;
            angle_y = y;

            xSemaphoreGive(imu_mutex);
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void setup() {
    // Configure serial transport
    Serial.begin(115200);
    set_microros_serial_transports(Serial);
    delay(2000);
    sensor.setThrottle();

    sensor.axe = 0.574;
    sensor.aye = -0.002;
    sensor.aze = -1.043;
    sensor.gxe = 10.702;
    sensor.gye = -6.436;
    sensor.gze = -0.676;

  imu_mutex = xSemaphoreCreateMutex();

    if (imu_mutex == NULL)
    {
        Serial.println("Erro ao criar mutex do IMU");
        while (true)
        {
            delay(1000);
        }
    }
    xTaskCreate(
        task_gy521,
        "GY521 Task",
        4096,
        NULL,
        2,
        &imu_task_handle
    );
  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_platformio_node", "", &support));

  // create publisher
  RCCHECK(rclc_publisher_init_default(
    &publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "micro_ros_platformio_node_publisher"));

  // create timer,
  const unsigned int timer_timeout = 1000;
  RCCHECK(rclc_timer_init_default(
    &timer,
    &support,
    RCL_MS_TO_NS(timer_timeout),
    timer_callback));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_timer(&executor, &timer));

  msg.data = 0;
}

void loop() {
  delay(100);
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}