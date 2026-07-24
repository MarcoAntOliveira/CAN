#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>

#if !defined(MICRO_ROS_TRANSPORT_ARDUINO_SERIAL)
#error This example is only avaliable for Arduino framework with serial transport.
#endif

rcl_publisher_t publisher;
std_msgs__msg__Int32 msg;

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;

// Definição dos pinos do ESP32 conectados ao driver de motor
#define IN1 13  // Canal A - Sentido 1
#define IN2 12  // Canal A - Sentido 2
#define IN3 27  // Canal B - Sentido 1
#define IN4 26  // Canal B - Sentido 2

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

void error_loop() {
  while(1) {
    delay(100);
  }
}

void moverFrente() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moverTras() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void pararMotores() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Estados dos Motores
enum EstadoMotor { FRENTE, PARADO_1, TRAS, PARADO_2 };
EstadoMotor estadoAtual = FRENTE;
unsigned long tempoUltimaTroca = 0;

void timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
  RCLC_UNUSED(last_call_time);
  if (timer == NULL) return;

  unsigned long agora = millis();

  // Máquina de estados baseada em tempo (Sem bloquear o executor)
  switch (estadoAtual) {
    case FRENTE:
      moverFrente();
      msg.data = 1;
      if (agora - tempoUltimaTroca >= 2000) { // Espera 2s
        estadoAtual = PARADO_1;
        tempoUltimaTroca = agora;
      }
      break;

    case PARADO_1:
      pararMotores();
      msg.data = 0;
      if (agora - tempoUltimaTroca >= 1000) { // Espera 1s
        estadoAtual = TRAS;
        tempoUltimaTroca = agora;
      }
      break;

    case TRAS:
      moverTras();
      msg.data = -1;
      if (agora - tempoUltimaTroca >= 2000) { // Espera 2s
        estadoAtual = PARADO_2;
        tempoUltimaTroca = agora;
      }
      break;

    case PARADO_2:
      pararMotores();
      msg.data = 0;
      if (agora - tempoUltimaTroca >= 2000) { // Espera 2s
        estadoAtual = FRENTE;
        tempoUltimaTroca = agora;
      }
      break;
  }

  // Publica o estado atual do motor no tópico ROS 2
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}

void setup() {
  // Configura a comunicação serial exclusiva para o micro-ROS
  Serial.begin(115200);
  set_microros_serial_transports(Serial);
  delay(2000);

  // Configura os pinos do driver como SAÍDA
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pararMotores();

  allocator = rcl_get_default_allocator();

  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  RCCHECK(rclc_node_init_default(&node, "motor_state", "", &support));

  RCCHECK(rclc_publisher_init_default(
    &publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "motor_state"));

  // Timer disparando a cada 100ms para atualizar o controle e publicar estado rápido
  const unsigned int timer_timeout = 100;
  RCCHECK(rclc_timer_init_default(
    &timer,
    &support,
    RCL_MS_TO_NS(timer_timeout),
    timer_callback));

  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_timer(&executor, &timer));

  msg.data = 0;
  tempoUltimaTroca = millis();
}

void loop() {
  delay(10);
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10)));
}