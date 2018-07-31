// rf22_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_RF22 class. RH_RF22 class does not provide for addressing or
// reliability, so you should only use RH_RF22 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf22_server
// Tested on Duemilanove, Uno with Sparkfun RFM22 wireless shield
// Tested on Flymaple with sparkfun RFM22 wireless shield
// Tested on ChiKit Uno32 with sparkfun RFM22 wireless shield

#include <RH_RF22.h>
#include <RHSoftwareSPI.h>
#include <SPI.h>

// software defined SPI rather than Hardware for custom pins
RHSoftwareSPI spi;

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

// Singleton instance of the radio driver
RH_RF22 rf22(4, 2, spi);
boolean sendFlag;

void setup() 
{
  spi.setPins(9, 8, 7);
  Serial.begin(57600);
  if (!rf22.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
  rf22.setTxPower(RH_RF22_TXPOW_20DBM); //MAX POWER
  sendFlag = false;
}

void loop()
{
  rf22.waitPacketSent();  
  uint8_t msgSize = Serial.available();
  Serial.println(msgSize);
  if(msgSize){
    uint8_t i = 0;
    uint8_t sendBuf[RH_RF22_MAX_MESSAGE_LEN];
    while(i < RH_RF22_MAX_MESSAGE_LEN && i<msgSize){
      sendBuf[i] = Serial.read();
      i++;
    }
    rf22.setHeaderId(i);
    rf22.waitPacketSent();  
    if(rf22.send(sendBuf, i)){
          
    } else {
      Serial.println("send Fail");
    }
    
    // if no more to send, stop transmitting
    sendFlag = true;
    if(!Serial.available()){
      sendFlag = false;
      Serial.println("false");
    }
  }
  
  // See if there is anything to recieve and we are not sending big files
  if (!sendFlag){
    Serial.println("uh");
    if(rf22.waitAvailableTimeout(2000))
    { 
      uint8_t mybuf[rf22.headerId()];
      uint8_t len = sizeof(mybuf);
      // Should be a reply message for us now   
      if (rf22.recv(mybuf, &len))
      {
        Serial.write((char*)mybuf, len);
        Serial.write(mybuf, len);
        Serial.println();
  //      Serial.println(rf22.lastRssi(), DEC);
      }
      else
      {
        Serial.println("recv failed");
      }
    }
  }
}

