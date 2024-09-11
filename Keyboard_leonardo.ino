#include "Keyboard.h"
#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 3
#define PIN 12
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

volatile uint8_t R;
volatile uint8_t G;
volatile uint8_t B;

bool IsKeyPress(int Pin) {
  return digitalRead(Pin) == LOW;
}

int ApiKeyPress(uint8_t key) {
  if (key == 0)
    return 0;
  delay(5);
  // if(RGB.r < 255){
  //   RGB.r++;
  // }
  return Keyboard.press(key);
  // return Keyboard.press(key);
}

int ApiKeyrelease(uint8_t key) {
  if (key == 0)
    return 0;
  return Keyboard.release(key);
}
//  Keyboard.release(KeyPress.k1);


struct __key {
  int k1;
  int k2;
  int k3;
};
unsigned long startMillis;
unsigned long currentMillis;


class Keyboard_inp {
public:
  volatile int Pin = 0;
  volatile __key KeyPress;
  volatile bool isSleep = 0;
  volatile bool StatusKey = 0;
  volatile bool _dstatus = false;
  String debug_data = "";
  Keyboard_inp() {
  }
  Keyboard_inp(int _Pin, __key _KeyPress, bool _isSleep) {
    Pin = _Pin;
    KeyPress = _KeyPress;
    isSleep = _isSleep;
  }

  void KeyPressInt() {

    if (IsKeyPress(Pin)) {

      currentMillis = millis();
      if (currentMillis - startMillis >= 500) {
        R = random(0, 255);
        G = random(0, 255);
        B = random(0, 255);
        startMillis = currentMillis;
      }
      if (StatusKey == false) {


        ApiKeyPress(KeyPress.k1);
        ApiKeyPress(KeyPress.k2);
        ApiKeyPress(KeyPress.k3);




        ApiKeyrelease(KeyPress.k3);
        ApiKeyrelease(KeyPress.k2);
        ApiKeyrelease(KeyPress.k1);
        // Keyboard.press(KeyPress);
        // Keyboard.release(KeyPress);
        _dstatus = !_dstatus;
        debug_data = "";
        debug_data += "Pin ";
        debug_data += String(Pin);
        // debug_data += " KeyPress ";
        // debug_data += String(KeyPress);
        // debug_data += "|";
        // debug_data += String((char)KeyPress);
        debug_data += " isSleep ";
        debug_data += String(isSleep);


        Serial.println(debug_data);

        StatusKey = true;
      }

    } else {
      StatusKey = false;
    }
  }
};
#define KEY_LEFT_CTRL 0x80
#define KEY_LEFT_SHIFT 0x81
//volatile Keyboard_inp key_{ 2, 75, true };
////'';lkjh,mnnm,nm,.nm,///////
volatile Keyboard_inp keys[9] = {
  { 2, { KEY_LEFT_CTRL, KEY_LEFT_SHIFT, (50) }, true },
  { 3, { KEY_LEFT_CTRL, KEY_LEFT_SHIFT, (49) }, true },

  { 9, { KEY_PRINT_SCREEN, 0, 0 }, true },
};
int count_arr = sizeof(keys) / sizeof(keys[0]);

void InitPins() {
  for (int i = 2; i < 10; i++) {
    pinMode(i, INPUT_PULLUP);
  }
}

void setup() {
  Serial.begin(2400);

  pixels.begin();
  pixels.clear();
  pixels.show();

  InitPins();
  Keyboard.begin();
}

void refresh_pixels() {

  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(R, G, B));
  }
  pixels.show();
}
void loop() {

  refresh_pixels();
  for (size_t i = 0; i < count_arr; i++) {
    keys[i].KeyPressInt();
  }
  Keyboard.end();
  delay(50);
}
