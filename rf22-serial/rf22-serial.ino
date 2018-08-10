// rf22_simple_client.ino
// -*- mode: C++ -*-
// Sketch for Bidirectional communication. Based on RadioHead Library
// using RH_RF22 Driver for RFM22 tranceiver. The code implements a
// state machine where the two devices are in either a rx-tx config,
// or both are in idle until one sends a message. The commands to 
// bing and end rx and tx modes are "send-it!" and "done-boi"

#include <RHReliableDatagram.h>
#include <RH_RF22.h>
#include <RHSoftwareSPI.h>
#include <SPI.h>


// software defined SPI rather than Hardware for custom pins
RHSoftwareSPI spi;

#define LOCAL_ADDRESS 1
#define REMOTE_ADDRESS 2

// Singleton instance of the radio driver
RH_RF22 rf22(4, 2, spi); // Slave select, interupt, spi object

// manager for the driver
RHReliableDatagram manager(rf22, LOCAL_ADDRESS);

boolean txMode;
boolean rxMode;
unsigned long timeWatch;

void setup() 
{
  spi.setPins(9, 8, 7); //miso, mosi, sck
  Serial.begin(57600);

  
  if (!manager.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36

  rf22.setTxPower(RH_RF22_TXPOW_20DBM); //MAX POWER
  rf22.setModemConfig(RH_RF22::GFSK_Rb57_6Fd28_8);

  rxMode = false;
  txMode = false;
  timeWatch = 0;
}

void loop()
{ 
  // are we transmitting lots of data
  while(txMode){
    // read 50 bytes from computer
    //timeWatch = millis();
    uint8_t i = 0;
    uint8_t sendBuf[RH_RF22_MAX_MESSAGE_LEN];
    while(i < RH_RF22_MAX_MESSAGE_LEN){
      if(Serial.available()){
        sendBuf[i] = Serial.read();
        Serial.write(sendBuf[i]);
        i++;
      }
    }

    
//    while(Serial.available()){
//      Serial.write(Serial.read());
//    }
    //Serial.flush();

    // check to see if it is the end of transmit mode
    if(sendBuf[0] == 'd' && sendBuf[1]=='o' && sendBuf[2]=='n' && sendBuf[3]=='e'){
      txMode = false;
      Serial.println("tx end detected");
    }

//    Serial.print(i);
    Serial.print(sizeof(sendBuf));
//    Serial.write(Serial.available());
//    Serial.print(RH_RF22_MAX_MESSAGE_LEN);
    // tell the computer to send the next 50 bytes
//    Serial.write('k');
//    Serial.write(sendBuf[0]);
//    Serial.write(sendBuf[1]);
//    Serial.write(sendBuf[2]);
//    Serial.write(sendBuf[3]);
    //timeWatch = millis();
    // send message && wait for ACK packet
    if(!manager.sendtoWait(sendBuf, sizeof(sendBuf), REMOTE_ADDRESS)){
      txMode = false;
      Serial.println("message was not received");
    } 
//    delay(100);
//    Serial.write('|');
//    Serial.print(timeWatch);
//    Serial.write('|');
//    Serial.print(millis());
//    Serial.write('|');
//    Serial.print(millis()-timeWatch);
  }


  uint8_t recvbuf[RH_RF22_MAX_MESSAGE_LEN];
  uint8_t lenrx = sizeof(recvbuf);
  // are we receiving lots of data
  while(rxMode){
    // check if we have a msg waiting for us
    if(manager.recvfromAck(recvbuf, &lenrx)){
        Serial.write(recvbuf, lenrx); // write the data we get to computer
  //      Serial.println(rf22.lastRssi(), DEC);
  
      if(recvbuf[0] == 'd'){
        if(recvbuf[1]=='o' && recvbuf[2]=='n' && recvbuf[3]=='e'){
          rxMode = false;
          Serial.println("rx mode end detected");
        }
      }
    }
  }

  
  // we are back in idle mode

  uint8_t buf[RH_RF22_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if(manager.recvfromAckTimeout(buf, &len, 1000)){ 
    Serial.write(buf, len);
    if(buf[0] == 's' && buf[1]=='e' && buf[2]=='n' && buf[3]=='d'){
      rxMode = true;
      Serial.println("rx mode started");
    }
  } else {
    if(Serial.available()){
      uint8_t i = 0;
      uint8_t sendBuf[RH_RF22_MAX_MESSAGE_LEN];
      while(i < RH_RF22_MAX_MESSAGE_LEN){ //msg must be 50 bytes
        if(Serial.available()){
          sendBuf[i] = Serial.read();
          i++;
        }
      }

      if(sendBuf[0] == 's' && sendBuf[1]=='e' && sendBuf[2]=='n' && sendBuf[3]=='d'){
        txMode = true;
        Serial.println("tx mode is on");
      }
      
      if(!manager.sendtoWait(sendBuf, sizeof(sendBuf), REMOTE_ADDRESS)){
        txMode = false;
        Serial.println("failed send from idle");
      }
    }
  }
}

