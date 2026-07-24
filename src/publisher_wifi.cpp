#include <Arduino.h>
#include <WiFi.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>

#include <micro_ros_platformio.h>
#include <uxr/client/profile/transport/custom/custom_transport.h>


extern "C" {
bool platformio_transport_open(struct uxrCustomTransport * transport);
bool platformio_transport_close(struct uxrCustomTransport * transport);
size_t platformio_transport_write(
    struct uxrCustomTransport * transport,
    const uint8_t * buf,
    size_t len,
    uint8_t * errcode);

size_t platformio_transport_read(
    struct uxrCustomTransport * transport,
    uint8_t * buf,
    size_t len,
    int timeout,
    uint8_t * errcode);
}

// --- CANAL PRÓPRIO GERADO PELO ESP32 ---
const char* ap_ssid = "ESP32_microROS";     // Nome da rede criada pelo ESP32
const char* ap_password = "12345678password"; // Senha da rede

// IP do seu PC quando ele se conecta no canal do ESP32 (Padrão do primeiro cliente: 192.168.4.2)
IPAddress agent_ip(192, 168, 4, 2); 
size_t agent_port = 8888;

rcl_publisher_t publisher;
std_msgs__msg__Int32 msg;

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;

// Definição dos pinos do ESP32 para o motor
#define IN1 13
#define IN2 12
#define IN3 27
#define IN4 26

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

void error_loop() {
  while(1) {
    delay(100);
  }
}

void moverFrente() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void moverTras() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}

void pararMotores() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, LOW);
}

enum EstadoMotor { FRENTE, PARADO_1, TRAS, PARADO_2 };
EstadoMotor estadoAtual = FRENTE;
unsigned long tempoUltimaTroca = 0;

void timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
  RCLC_UNUSED(last_call_time);
  if (timer == NULL) return;

  unsigned long agora = millis();

  switch (estadoAtual) {
    case FRENTE:
      moverFrente();
      msg.data = 1;
      if (agora - tempoUltimaTroca >= 2000) {
        estadoAtual = PARADO_1;
        tempoUltimaTroca = agora;
      }
      break;

    case PARADO_1:
      pararMotores();
      msg.data = 0;
      if (agora - tempoUltimaTroca >= 1000) {
        estadoAtual = TRAS;
        tempoUltimaTroca = agora;
      }
      break;

    case TRAS:
      moverTras();
      msg.data = -1;
      if (agora - tempoUltimaTroca >= 2000) {
        estadoAtual = PARADO_2;
        tempoUltimaTroca = agora;
      }
      break;

    case PARADO_2:
      pararMotores();
      msg.data = 0;
      if (agora - tempoUltimaTroca >= 2000) {
        estadoAtual = FRENTE;
        tempoUltimaTroca = agora;
      }
      break;
  }

  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}

void setup() {
    Serial.begin(115200);
    delay(3000);

    Serial.println("1");

    WiFi.softAP(ap_ssid, ap_password);

    Serial.println("2");

    Serial.println(WiFi.softAPIP());

    Serial.println("3");

    delay(5000);

    Serial.println("4");
    Serial.println("5");


    static micro_ros_agent_locator locator;

    locator.address = agent_ip;
    locator.port = agent_port;

    rmw_uros_set_custom_transport(
        false,
        (void *)&locator,
        platformio_transport_open,
        platformio_transport_close,
        platformio_transport_write,
        platformio_transport_read
    );
    Serial.println("6");
    allocator = rcl_get_default_allocator();

    Serial.println("7");

    while (rclc_support_init(&support, 0, NULL, &allocator) != RCL_RET_OK) {
        Serial.println("Tentando conectar ao Agent...");
        delay(1000);
    }

    Serial.println("8 - Agent conectado!");

    RCCHECK(rclc_node_init_default(&node, "motor_state", "", &support));

    RCCHECK(rclc_publisher_init_default(
        &publisher,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        "motor_state"));

    const unsigned int timer_timeout = 100;

    RCCHECK(rclc_timer_init_default(
        &timer,
        &support,
        RCL_MS_TO_NS(timer_timeout),
        timer_callback));

    RCCHECK(rclc_executor_init(
        &executor,
        &support.context,
        1,
        &allocator));

    RCCHECK(rclc_executor_add_timer(&executor, &timer));

    tempoUltimaTroca = millis();
    msg.data = 0;

Serial.println("9 - Tudo inicializado!");
  }
void loop() {
  delay(10);
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10)));
}