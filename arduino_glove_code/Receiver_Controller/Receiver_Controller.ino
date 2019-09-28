#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00002";

int Data[3];

int LED_PIN1 = 8;
int LED_PIN2 = 7;

void setup() 
{
  Serial.begin(9600);
  
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  
  digitalWrite(LED_PIN1, LOW);
  digitalWrite(LED_PIN2, LOW);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
}

void loop() 
{
  if (radio.available()) 
  {
    if (Data[0] > 50 && Data[0] <= 255) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("LDR");
    digitalWrite(LED_PIN1, HIGH);
    digitalWrite(LED_PIN2, LOW);
    delay(100);
  }

   else if (Data[1] > 300 && Data[1] <= 400) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println(Data);
    
    delay(100);
  }

  else if (Data[2] <= 5) 
  {
    radio.read(Data, sizeof(Data));
    Serial.println(Data);
    digitalWrite(LED_PIN2, HIGH);
    digitalWrite(LED_PIN1, LOW);
    delay(100);
  }

  else 
  {
    radio.read(Data, sizeof(Data));
    Serial.println("Normal");
    digitalWrite(LED_PIN2, LOW);
    digitalWrite(LED_PIN1, LOW);
    
    delay(100);
  }
  }
}
