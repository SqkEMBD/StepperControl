#include <Arduino.h>
#include <Stepper.h>
#include "main.h"

#define BUTTON_PIN PB2
#define NOT_RUNNING 0 
#define RUNNING 1

enum MachineState
{
  MOTOR_HALT = 1,
  CLOCKWISE,
  COUNTERCLOCKWISE,
};

enum ButtonState
{
  NOT_PRESSED = 0,
  PRESSED,
};

/**
 * @brief Global variables section.
 * 
 */
int stepCount = 0;         // number of steps the motor has taken
int labNumber = 0;
int switchEvent = 0;       //
bool buttonState = NOT_PRESSED;  // button state initiilze.
int labState = NOT_RUNNING;
// int currentState = MOTOR_HALT; //

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup()
{
  // initialize the serial port:
  Serial.begin(9600);

  // initialize button switch:
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize LED built-in:
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  // button if switch is short or pressed it will return HIGH or '1', otherwise will be LOW or '0'
  checkButton();
  switch (labNumber)
  {
    case 1:
      Serial.println("labNumber 1:");
      motorHalt();
    break;
    case 2:
      Serial.println("labNumber 2:");
      rotateCW();
    break;
    case 3:
      Serial.println("labNumber 3:");
      rotateCCW();
    break;
    case 4:
      Serial.println("labNumber 4:");
    break;
  }

  if(switchEvent)
  {
    nextState();
  }

}

void checkButton(void)
{
  if (buttonState == NOT_PRESSED)
  {
    if (!(digitalRead(BUTTON_PIN) == LOW))
    {
      delay(100);
      buttonState = PRESSED;
      Serial.println("buttonState = Pressed");
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  else if (!(digitalRead(BUTTON_PIN) == HIGH))
  {
    buttonState = NOT_PRESSED;
    switchEvent = 1;
    Serial.println("buttonState = UnPressed");
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void nextState(void)
{
  switchEvent = 0;
  labNumber++;
  
  if(labNumber > 4)
  {
    labNumber = 1;
  }
}

void motorHalt(void)
{
  Serial.println("Motor Halt");
}

void rotateCW(void)
{
  if (labState == NOT_RUNNING)
  {
    labState = RUNNING;
  }
  if (labState == RUNNING)
  {
    //* Clock-wise (CW)
    // digitalWrite(LED_BUILTIN, HIGH);
    myStepper.step(1);
    Serial.print("CW steps:");
    Serial.println(stepCount);
    stepCount++;
    delay(1);
  }
  if (switchEvent)
  {
    labState = NOT_RUNNING;
  }
}

void rotateCCW(void)
{
  if (labState == NOT_RUNNING)
  {
    labState = RUNNING;
  }
  if (labState == RUNNING)
  {
    //* Counter Clock-wise (CCW)
    // digitalWrite(LED_BUILTIN, HIGH);
    myStepper.step(-1);
    Serial.print("CCW steps:");
    Serial.println(stepCount);
    stepCount--;
    delay(1);
  }
  if (switchEvent)
  {
    labState = NOT_RUNNING;
  }
}
