#include <JoystickShield.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

JoystickShield joystickShield;

int Data[15];

void setup() 
{
  Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
}

void loop() 
{
  joystickShield.processEvents();
  
  if (radio.available()) 
  {
    if (Data[0] == HIGH) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("UP");

    delay(200);
  } 
    
    else if (Data[1] == HIGH) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("RightUp");
    
    delay(200);
  }
  
    else if (Data[2] == HIGH) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("Right");
    
    delay(200);
  } 
  
    else if (Data[3] == HIGH) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("RightDown");
    
    delay(200);
  } 

    else if (Data[4] == HIGH) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("Down");
    
    delay(200);
  } 

    else if (Data[5] == HIGH) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("LeftDown");
    
    delay(200);
  }

    else if (Data[6] == HIGH) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("Left");
    
    delay(200);
  }

    else if (Data[7] == HIGH) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("LeftUp");
    
    delay(200);
  }

    else if (Data[8] == HIGH) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("Joystick Clicked");
    
    delay(200);
  }

    else if (Data[9] == HIGH) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("A Clicked");
    
    delay(200);
  }

    else if (Data[10] == HIGH) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("B Clicked");
    
    delay(200);
  }

    else if (Data[11] == HIGH) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("C Clicked");
    
    delay(200);
  }

    else if (Data[12] == HIGH) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("D Clicked");
    
    delay(200);
  }

    else if (Data[13] == HIGH) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("E Clicked");
    
    delay(200);
  }

    else if (Data[14] == HIGH) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("F Clicked");
    
    delay(200);
  }
  
   else 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("Normal");
    
    delay(200);
  }
  }
}
