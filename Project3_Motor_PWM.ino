#include <Arduino.h>

#define BUTTON_PIN 4
#define PWM_PIN 9   

volatile int pwmCounter = 0;

const int NUM_STEPS = 8;
int dutySteps[NUM_STEPS] = {0, 2, 4, 6, 8, 6, 4, 2};
int dutyIndex = 0;

bool lastButton = HIGH;
unsigned long lastDebounce = 0;

void taskButton();
void taskMotorPWM();
void taskEmptyPWM();  

void (*taskQueue[])(void) = {
  taskButton,
  taskMotorPWM,
  taskEmptyPWM,
  NULL
};

void setupTimer1() {

  cli(); 

  TCCR1A = 0;
  TCCR1B = 0;

  TCCR1B |= (1 << WGM12);

  OCR1A = 1999;    

  TIMSK1 |= (1 << OCIE1A); 

  TCCR1B |= (1 << CS10);   

  sei(); 
}

ISR(TIMER1_COMPA_vect) {

  pwmCounter++;

  if (pwmCounter < dutySteps[dutyIndex])
    digitalWrite(PWM_PIN, HIGH);
  else
    digitalWrite(PWM_PIN, LOW);

  if (pwmCounter >= 8)
    pwmCounter = 0;
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(PWM_PIN, OUTPUT);

  setupTimer1();

  Serial.println("Project 3 Ready — Press button to change motor speed.");
}

void loop() {
  int i = 0;
  while (taskQueue[i] != NULL) {
    (*taskQueue[i])();
    i++;
  }
}

void taskButton() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading == LOW && lastButton == HIGH && (millis() - lastDebounce) > 50) {

    dutyIndex++;
    if (dutyIndex >= NUM_STEPS) dutyIndex = 0;

    Serial.print("Duty step: ");
    Serial.print(dutySteps[dutyIndex]);
    Serial.println("/8");

    lastDebounce = millis();
  }

  lastButton = reading;
}

void taskMotorPWM() {
}

void taskEmptyPWM() {
}
