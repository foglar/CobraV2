#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 8);                 // CE, CSN
const byte address[6] = "00001";  //address through which two modules communicate


void setup() {
  while (!Serial)
    ;
  Serial.begin(9600);
  Serial.println("#200 Reciever Init");
  radio.begin();
  radio.openReadingPipe(0, address);  //set the address
  radio.startListening();             //Set module as receiver
}


void loop() {
  //Read the data if available in buffer
  if (radio.available()) {
    char text[64] = { 0 };
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
}
