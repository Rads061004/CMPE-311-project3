#include <Arduino.h>

#define PWM_PIN 9
#define BUTTON_PIN 4

const int NUM_LEVELS = 6;  
int dutyLevels[NUM_LEVELS] = {0, 51, 102, 153, 204, 255};  
int currentLevel = 0;

bool lastButton = HIGH;  
unsigned long lastDebounce = 0;

void ledTask() {
  analogWrite(PWM_PIN, dutyLevels[currentLevel]);
}

void buttonTask() {
  bool reading = digitalRead(BUTTON_PIN);
  
  if (reading == LOW && lastButton == HIGH && (millis() - lastDebounce) > 50) {
    currentLevel = (currentLevel + 1) % NUM_LEVELS;

    Serial.print("Brightness level: ");
    Serial.print((currentLevel * 100) / (NUM_LEVELS - 1));
    Serial.println("%");

    lastDebounce = millis();
  }

  lastButton = reading;
}

typedef void (*TaskFn)();
TaskFn tasks[] = { ledTask, buttonTask };
const int NUM_TASKS = 2;

void setup() {
  Serial.begin(9600);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("System Ready. Press button to change brightness.");
}

void loop() {
  for (int i = 0; i < NUM_TASKS; i++) {
    tasks[i]();
  }
}
