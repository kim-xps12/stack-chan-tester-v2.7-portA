 #include <Arduino.h>

#include <SD.h>
#include <Ticker.h>
#include <M5StackUpdater.h>
#include <M5Unified.h>

#define SERVO_PIN_X 22
#define SERVO_PIN_Y 21

int servo_offset_x = 0;  // X軸サーボのオフセット（90°からの+-で設定）
int servo_offset_y = 0;  // Y軸サーボのオフセット（90°からの+-で設定）

#include <Avatar.h> // https://github.com/meganetaaan/m5stack-avatar
#include <ServoEasing.hpp> // https://github.com/ArminJo/ServoEasing       
#include "formatString.hpp" // https://gist.github.com/GOB52/e158b689273569357b04736b78f050d6

using namespace m5avatar;
Avatar avatar;

#define START_DEGREE_VALUE_X 90
#define START_DEGREE_VALUE_Y 90

#define SDU_APP_PATH "/stackchan_tester.bin"
#define TFCARD_CS_PIN 4

ServoEasing servo_x;
ServoEasing servo_y;

uint32_t mouth_wait = 2000; // 通常時のセリフ入れ替え時間（msec）
uint32_t last_mouth_millis = 0;

const char* lyrics[] = { "BtnA:MoveTo90  ", "BtnB:ServoTest  ", "BtnC:RandomMode  ", "BtnALong:AdjustMode"};
const int lyrics_size = sizeof(lyrics) / sizeof(char*);
int lyrics_idx = 0;

void moveX(int x, uint32_t millis_for_move = 0) {
  if (millis_for_move == 0) {
    servo_x.easeTo(x + servo_offset_x);
  } else {
    servo_x.easeToD(x + servo_offset_x, millis_for_move);
  }
}

void moveY(int y, uint32_t millis_for_move = 0) {
  if (millis_for_move == 0) {
    servo_y.easeTo(y + servo_offset_y);
  } else {
    servo_y.easeToD(y + servo_offset_y, millis_for_move);
  }
}

void moveXY(int x, int y, uint32_t millis_for_move = 0) {
  if (millis_for_move == 0) {
    servo_x.setEaseTo(x + servo_offset_x);
    servo_y.setEaseTo(y + servo_offset_y);
  } else {
    servo_x.setEaseToD(x + servo_offset_x, millis_for_move);
    servo_y.setEaseToD(y + servo_offset_y, millis_for_move);
  }
  // サーボが停止するまでウェイトします。
  synchronizeAllServosStartAndWaitForAllServosToStop();
}

void adjustOffset() {
  // サーボのオフセットを調整するモード
  servo_offset_x = 0;
  servo_offset_y = 0;
  moveXY(90, 90);
  bool adjustX = true;
  for (;;) {

    M5.update();
    if (M5.BtnA.wasPressed()) {
      // オフセットを減らす
      if (adjustX) {
        servo_offset_x--;
      } else {
        servo_offset_y--;
      }
    }
    if (M5.BtnB.pressedFor(2000)) {
      // 調整モードを終了
      break;
    }
    if (M5.BtnB.wasPressed()) {
      // 調整モードのXとYを切り替え
      adjustX = !adjustX;
    }
    if (M5.BtnC.wasPressed()) {
      // オフセットを増やす
      if (adjustX) {
        servo_offset_x++;
      } else {
        servo_offset_y++;
      }
    }
    moveXY(90, 90);

    std::string s;

    if (adjustX) {
      s = formatString("%s:%d:BtnB:X/Y", "X", servo_offset_x);
    } else {
      s = formatString("%s:%d:BtnB:X/Y", "Y", servo_offset_y);
    }
    avatar.setSpeechText(s.c_str());

  }
}

void moveRandom() {
  for (;;) {
    // ランダムモード
    int x = random(45, 135);  // 45〜135° でランダム
    int y = random(60, 90);   // 50〜90° でランダム

    M5.update();
    if (M5.BtnC.wasPressed()) {
      break;
    }
    int delay_time = random(10);
    moveXY(x, y, 1000 + 100 * delay_time);
    delay(2000 + 500 * delay_time);
    //avatar.setBatteryStatus(M5.Power.isCharging(), M5.Power.getBatteryLevel());
    //avatar.setSpeechText("Stop BtnC");
    avatar.setSpeechText("");
  }
}
void testServo() {
  for (int i=0; i<2; i++) {
    avatar.setSpeechText("X 90 -> 0  ");
    moveX(0);
    avatar.setSpeechText("X 0 -> 180  ");
    moveX(180);
    avatar.setSpeechText("X 180 -> 90  ");
    moveX(90);
    avatar.setSpeechText("Y 90 -> 50  ");
    moveY(50);
    avatar.setSpeechText("Y 50 -> 90  ");
    moveY(90);
  }
}

void setup() {
  auto cfg = M5.config();     // 設定用の情報を抽出
  //cfg.output_power = true;   // Groveポートの出力をしない
  
  M5.begin(cfg);              // M5Stackをcfgの設定で初期化
  M5.In_I2C.release();

  M5.Log.setLogLevel(m5::log_target_display, ESP_LOG_NONE);
  M5.Log.setLogLevel(m5::log_target_serial, ESP_LOG_INFO);
  M5.Log.setEnableColor(m5::log_target_serial, false);
  M5_LOGI("Hello World");
  Serial.println("HelloWorldSerial");

  if (servo_x.attach(SERVO_PIN_X, 
                     START_DEGREE_VALUE_X + servo_offset_x,
                     DEFAULT_MICROSECONDS_FOR_0_DEGREE,
                     DEFAULT_MICROSECONDS_FOR_180_DEGREE)) {
    Serial.print("Error attaching servo x");
  }
  if (servo_y.attach(SERVO_PIN_Y,
                     START_DEGREE_VALUE_Y + servo_offset_y,
                     DEFAULT_MICROSECONDS_FOR_0_DEGREE,
                     DEFAULT_MICROSECONDS_FOR_180_DEGREE)) {
    Serial.print("Error attaching servo y");
  }
  servo_x.setEasingType(EASE_QUADRATIC_IN_OUT);
  servo_y.setEasingType(EASE_QUADRATIC_IN_OUT);
  setSpeedForAllServos(60);
  avatar.init();
  last_mouth_millis = millis();
  avatar.setBatteryIcon(false);
  //moveRandom();
  //testServo();
}

void loop() {
  M5.update();
  if (M5.BtnA.pressedFor(2000)) {
    // サーボのオフセットを調整するモードへ
    adjustOffset();
  }
  if (M5.BtnA.wasPressed()) {
    moveXY(90, 90);
  }
  
  if (M5.BtnB.wasPressed()) {
    testServo();
  } 
  if (M5.BtnC.pressedFor(5000)) {
    M5_LOGI("Will copy this sketch to filesystem");
    if (saveSketchToFS( SD, SDU_APP_PATH, TFCARD_CS_PIN )) {
      M5_LOGI("Copy Successful!");
    } else {
      M5_LOGI("Copy failed!");
    }
  } else if (M5.BtnC.wasPressed()) {
    // ランダムモードへ
    moveRandom();
  }

  if ((millis() - last_mouth_millis) > mouth_wait) {
    const char* l = lyrics[lyrics_idx++ % lyrics_size];
    avatar.setSpeechText(l);
    avatar.setMouthOpenRatio(0.7);
    delay(200);
    avatar.setMouthOpenRatio(0.0);
    last_mouth_millis = millis();
    //avatar.setBatteryStatus(M5.Power.isCharging(), M5.Power.getBatteryLevel());
  }

}
