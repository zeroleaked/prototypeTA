#include "esp_camera.h"

//
// WARNING!!! PSRAM IC required for UXGA resolution and high JPEG quality
//            Ensure ESP32 Wrover Module or other board with PSRAM is selected
//            Partial images will be transmitted if image exceeds buffer size
//

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM

#include "camera_pins.h"
#include "tatypes.h"

esp_err_t command_handler(command_t command, uint16_t arg1, blank_t * blank);

// globals
char input_buffer[255];
uint8_t input_buffer_len = 0;
blank_t blank;
float blank_buf[ROW_LENGTH];

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;
  
  config.frame_size = FRAMESIZE_XGA;
  config.fb_count = 1;

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  Serial.println("Camera init OK");

  sensor_t * s = esp_camera_sensor_get();

  int res = 0;

  res = s->set_exposure_ctrl(s, 0);
  res = s->set_raw_gma(s, 0);
  res = s->set_whitebal(s, 0);
  res = s->set_awb_gain(s, 0);
  res = s->set_gain_ctrl(s, 0);
  
  blank.buf = blank_buf;
}

void loop() {
  if (Serial.available() > 0) {
    char input_char = Serial.read();

    if (input_char == '\n') {
      input_buffer[input_buffer_len] = '\0';
      input_buffer_len = 0;

      char command_char;
      uint16_t arg1;
      sscanf(input_buffer, "%c %d", &command_char, &arg1);
      
      command_t command = NO_COM;
      switch (command_char) {
        case 'I': {
          command = FIXED_EXPOSURE_COM;
          break;
        }
        case 'B': {
          command = BLANK_COM;
          break;
        }
        case 'A': {
          command = ABSORBANCE_COM;
          break;
        }
        case 'E': {
          command =  VAR_EXPOSURE_COM;
          break;
        }
        case 'R': {
          command =  ROW_DETECT_COM;
          break;
        }
        case 'G': {
          command =  VAR_GAIN_COM;
          break;
        }
        case 'V': {
          command =  BLANK_FILTER_COM;
          break;
        }
        case 'S': {
          command =  ABSORBANCE_FILTER_COM;
          break;
        }
        default: {
          Serial.println("Command unrecognized");
        }
      }

      command_handler(command, arg1, &blank);

      // if (command == BLANK_COM) {
      //   Serial.println(blank.buf[ROW_LENGTH-1]);
      // }
    }
    else {
      input_buffer[input_buffer_len] = input_char;
      input_buffer_len++;
    }
  }
}


