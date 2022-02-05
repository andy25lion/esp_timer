#include "esphome.h"
using namespace esphome;

class MyCustomComponent : public PollingComponent, public Sensor {
  private:

  uint8_t digit1pins[7] = {15, 2, 4, 16, 17, 5, 18};//{18, 5, 17, 16, 4, 2, 15}; 
  uint8_t digit2pins[7] = {13, 12, 14, 27, 26, 25, 33};//{33, 25, 26, 27, 14, 12, 13};

  uint8_t digits[10][7] = {
      {1, 1, 1, 1, 1, 1, 0}, //0
      {0, 0, 1, 1, 0, 0, 0}, //1
      {1, 1, 0, 1, 1, 0, 1}, //2
      {0, 1, 1, 1, 1, 0, 1}, //3
      {0, 0, 1, 1, 0, 1, 1}, //4
      {0, 1, 1, 0, 1, 1, 1}, //5
      {1, 1, 1, 0, 1, 1, 1}, //6
      {0, 0, 1, 1, 1, 0, 0}, //7
      {1, 1, 1, 1, 1, 1, 1}, //8
      {0, 1, 1, 1, 1, 1, 1}  //9
    };
  
  public:
  int count;
  bool isRunning;
  bool isPowerOn;

  float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }

  void setup() override {
    for (uint8_t i = 0; i < 7; i++) {
      pinMode(digit1pins[i], OUTPUT);
      pinMode(digit2pins[i], OUTPUT);
    }
    count = id(totalTime) * 60;
    isRunning = false;
    isPowerOn = false;
  }  MyCustomComponent() : PollingComponent(1000) { }

  void loop() override {
  }

  void update() override {
    publish_state(count/60);
  }

  void tick() {
    if (!isPowerOn) {
      powerOff();
      return;
    }
    if (count >= 0) {
      display7SegmentNumber(count/60);
      if (isRunning)
        count--;
    } else {
      isRunning = false;
    }
  }

  void display7SegmentNumber(int number) {
    int digit1 = number / 10 % 10;
    int digit2 = number % 10;
    for (uint8_t i = 0; i < 7; i++) {
      digitalWrite(digit1pins[i], digits[digit1][i]);
      digitalWrite(digit2pins[i], digits[digit2][i]);
    }
  }

  void start() {
    isRunning = true;
    if (count < 0) {
      reset();
    }
  }
  void stop() {
    isRunning = false;
    count = -1;
  }
  void pause() {
    isRunning = false;
  }
  void reset() {
    count = id(totalTime) * 60;
  }
  void powerOff() {
    for (uint8_t i = 0; i < 7; i++) {
      digitalWrite(digit1pins[i], LOW);
      digitalWrite(digit2pins[i], LOW);
    }
  }

  void test() {
    // if (count%2) {
    //   for (uint8_t i = 0; i < 7; i++) {
    //     pinMode(digit1pins[i], OUTPUT);
    //     digitalWrite(digit1pins[i], LOW);
    //     pinMode(digit2pins[i], OUTPUT);
    //     digitalWrite(digit2pins[i], LOW);
    //   }
    // } else {
    //   for (uint8_t i = 0; i < 7; i++) {
    //     pinMode(digit1pins[i], INPUT);
    //     digitalWrite(digit1pins[i], HIGH);
    //     pinMode(digit2pins[i], INPUT);
    //     digitalWrite(digit2pins[i], HIGH);
    //   }
    // }

    for (uint8_t i = 0; i < 7; i++) {
      digitalWrite(digit1pins[i], count%2);
    }
    for (uint8_t i = 0; i < 7; i++) {
      digitalWrite(digit2pins[i], count%2);
    }
    count++;
  }
};


auto my_timer = new MyCustomComponent();