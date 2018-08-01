// rf22_simple_client.ino
// -*- mode: C++ -*-
// Sketch for Bidirectional communication. Based on RadioHead Library
// using RH_RF22 Driver for RFM22 tranceiver. The code implements a
// state machine where the two devices are in either a rx-tx config,
// or both are in idle until one sends a message. The commands to 
// bing and end rx and tx modes are "send-it!" and "done-boi"

#include <RH_RF22.h>
#include <RHSoftwareSPI.h>
#include <SPI.h>

// software defined SPI rather than Hardware for custom pins
RHSoftwareSPI spi;

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

// Singleton instance of the radio driver
RH_RF22 rf22(4, 2, spi);

boolean txMode;
boolean rxMode;
uint8_t endFlag[] = "done-boi";

void setup() 
{
  spi.setPins(9, 8, 7);
  Serial.begin(115200);
  if (!rf22.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
  rf22.setTxPower(RH_RF22_TXPOW_20DBM); //MAX POWER
  rxMode = false;
  txMode = false;
}

void loop()
{ 
  // are we transmitting lots of data
  while(txMode){
    //uint8_t msgSize = Serial.available(); // get the number of bytes coming in 
    //Serial.println(msgSize);
    if(Serial.available()){
      uint8_t i = 0;
      uint8_t j = 0;
      uint8_t sendBuf[RH_RF22_MAX_MESSAGE_LEN];
      while(i < RH_RF22_MAX_MESSAGE_LEN && Serial.available() && j<7){
        sendBuf[i] = Serial.read();
        if(sendBuf[i] == endFlag[j]){
          j++;
        } else {
          j=0;
        }
        i++;
      }
      
//      if(sendBuf[0] == 'd' && sendBuf[1]=='o' && sendBuf[2]=='n' && sendBuf[3]=='e' && sendBuf[4]=='-'){
//        txMode = false;
//        Serial.println("tx end detected");
//      }
      

      // send message
      rf22.setHeaderId(i);  // not sure what ID is usually for, we are using it for msg size
      if(rf22.send(sendBuf, &i)){
        rf22.waitPacketSent();  // EXPERIMENT TO SEE IF B4 is better     
      } else {
        txMode = false;
        Serial.println("send Fail during tx");
      }
    }
  }

  // are we receiving lots of data
  while(rxMode){
    // check if we have a msg waiting for us
    if(rf22.available()){
      uint8_t mybuf[rf22.headerId()]; // get msg size
      uint8_t len = sizeof(mybuf);  // 
      if (rf22.recv(mybuf, &len)){
        Serial.write(mybuf, len);
        Serial.println("----"); // for debug
  //      Serial.println(rf22.lastRssi(), DEC);
      } else {
        Serial.println("recv failed");
      }

      // if incoming message is "done-boi", turn of receive mode
      if(len == 8){
        if(mybuf[0] == 'd' && mybuf[1]=='o' && mybuf[2]=='n' && mybuf[3]=='e' && mybuf[4]=='-'){
          rxMode = false;
          Serial.println("tx end detected");
        }
      }
      
    }
  }

  // we are back in idle mode
  if(rf22.waitAvailableTimeout(1000)){ 
    uint8_t mybuf[rf22.headerId()];
    uint8_t len = sizeof(mybuf);
    // Should be a reply message for us now   
    if (rf22.recv(mybuf, &len))
    {
      Serial.write(mybuf, len);
      Serial.println("--------");
      if(len == 8){
        if(mybuf[0] == 's' && mybuf[1]=='e' && mybuf[2]=='n' && mybuf[3]=='d' && mybuf[4]=='-'){
          rxMode = true;
          Serial.println("rx start detected");
        }
      }
//      Serial.println(rf22.lastRssi(), DEC);
    }
    else
    {
      Serial.println("recv failed");
    }
  } else {
    uint8_t msgSize = Serial.available(); // get the number of bytes coming in 
    if(msgSize){
      uint8_t i = 0;
      uint8_t sendBuf[RH_RF22_MAX_MESSAGE_LEN];
      while(i < RH_RF22_MAX_MESSAGE_LEN && i<msgSize){
        sendBuf[i] = Serial.read();
        i++;
      }

      // check if it was the stop signal
      if(msgSize == 8){
        if(sendBuf[0] == 's' && sendBuf[1]=='e' && sendBuf[2]=='n' && sendBuf[3]=='d' && sendBuf[4]=='-'){
          txMode = true;
          Serial.println("tx start detected");
        }
      }

      // send message
      rf22.setHeaderId(i);  // not sure what ID is usually for, we are using it for msg size
      if(rf22.send(sendBuf, i)){
        rf22.waitPacketSent();  // EXPERIMENT TO SEE IF B4 is better     
      } else {
        txMode = false;
        Serial.println("send Fail from idle");
      }
    }
  }
}

