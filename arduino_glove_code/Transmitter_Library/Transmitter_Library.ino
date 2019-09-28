#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <JoystickShield.h>

RF24 radio(9, 10); // CE, CSN

int Data[15];

JoystickShield joystickShield;

const byte address[6] = "00001";

void setup() 
{
  Serial.begin(9600);
  
  joystickShield.calibrateJoystick();
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
}

void loop() 
{
  joystickShield.processEvents();
  
  Data[0] = joystickShield.isUp();
  Data[1] = joystickShield.isRightUp();
  Data[2] = joystickShield.isRight();
  Data[3] = joystickShield.isRightDown();
  Data[4] = joystickShield.isDown();
  Data[5] = joystickShield.isLeftDown();
  Data[6] = joystickShield.isLeft();
  Data[7] = joystickShield.isLeftUp();
  Data[8] = joystickShield.isJoystickButton();
  Data[9] = joystickShield.isUpButton();
  Data[10] = joystickShield.isRightButton();
  Data[11] = joystickShield.isDownButton();
  Data[12] = joystickShield.isLeftButton();
  Data[13] = joystickShield.isEButton();
  Data[14] = joystickShield.isFButton();

  radio.write(Data, sizeof(Data));
}

