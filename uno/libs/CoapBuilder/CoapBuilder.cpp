/*
  CoapPraser.cpp - Library for parsing CoAP messages.
  Created by Kamil Stasiak, 2017.
  Implements only selected parts of the CoAP protocol.
*/
#include "Arduino.h"
#include "CoapBuilder.h"

//mozna jeszcze sprawdzanie czy nie przekroczono maksymalnej dlugosci zrobic

CoapBuilder::CoapBuilder(){
  message[4] = '\0';
  }

void CoapBuilder::init()
{
  //ver = 1, NON, no token
  message[0] = 0x50;
  //code 2.05 content
  message[1] = 0x85;
  //message id = 0
  message[2] = 0;
  message[3] = 0;
  message[4] = '\0';
  _lastOptionStart = 4;
}

void CoapBuilder::setVersion(uint8_t value)
{
 
	Serial.println(F("**[BUILDER][setVersion]:"));
	Serial.println(message[0] & 0x3f);
	Serial.println((value & 0x03) << 6);
	
	  message[0] = (message[0] & 0x3f) + ((value & 0x03) << 6);
	
	Serial.println(message[0],BIN);
	Serial.println(F("**[BUILDER][setVersion]:END"));
	

}

void CoapBuilder::setType(uint8_t value)
{
  	Serial.println(F("**[BUILDER][setType]:"));
	Serial.println(message[0] & 0xcf);
	Serial.println((value & 0x03) << 4);
	
	    message[0] = (message[0] & 0xcf) + ((value & 0x03) << 4);
	
	
	Serial.println(message[0],BIN);
	Serial.println(F("**[BUILDER][setType]:END"));
}

void  CoapBuilder::setTokenLen(uint8_t value)
{
    Serial.println(F("**[BUILDER][setTokenLen]:"));
	Serial.println(message[0] & 0xf0);
	Serial.println(value & 0x0f);
	
	message[0] = (message[0] & 0xf0) + (value & 0x0f);
	
	Serial.println(message[0],BIN);
	Serial.println(F("**[BUILDER][setTokenLen]:END"));
}

void CoapBuilder::setCodeClass(uint8_t value)
{
    Serial.println(F("**[BUILDER][setCodeClass]:"));
	Serial.println(message[1] & 0x1f);
	Serial.println((value & 0x07) << 5);

	message[1] = (message[1] & 0x1f) + ((value & 0x07) << 5); 

	Serial.println(message[1],BIN);
	Serial.println(F("**[BUILDER][setCodeClass]:END")); 	
}

void CoapBuilder::setCodeDetail(uint8_t value)
{
    Serial.println(F("**[BUILDER][setCodeDetail]:"));
	Serial.println(message[1],BIN);
	Serial.println(message[1] &  0xe0);
	Serial.println(value & 0x1f);
	
	message[1] = (message[1] &  0xe0) + (value & 0x1f);
	
	Serial.println(message[1],BIN);
	Serial.println(F("**[BUILDER][setCodeDetail]:END")); 
}

void CoapBuilder::setMessageId(uint16_t value)
{ 
    Serial.println(F("**[BUILDER][setMessageId]:"));
	Serial.println(value & 0xff00);
	Serial.println((value & 0xff00) >> 8);
	Serial.println(value & 0x00ff);

  message[2] = (value & 0xff00) >> 8;
  message[3] = value & 0x00ff;	
  
  	Serial.println(message[2],BIN);
	Serial.println(message[3],BIN);
	Serial.println(F("**[BUILDER][setMessageId]:END"));
}

void CoapBuilder::setToken(char* value)
{
  uint8_t tokenLen = (uint8_t) strlen(value);
  setTokenLen(tokenLen);
  uint8_t  messageLen = strlen(message);
  //shift if options or payload are set
  if (messageLen > 4) {
    //from message[messageLen - 1 + tokenLen] = message[messageLen - 1]
    //to message[messageLen - messageLen + 4 + tokenLen] = message[messageLen - messageLen + 4]
    for (uint8_t j = 1; j > messageLen - 5; j++) {
      message[messageLen - j + tokenLen] = message[messageLen - j];
    }
  } else {
    message[4 + tokenLen] = '\0';
  }
  _lastOptionStart += tokenLen;
  
  //set token value
  for (int i = 0; i < strlen(value); i++) {
    message[4 + i] = value[i];
  }
  
	Serial.println(F("**[BUILDER][setToken]:Hole Message"));	
	size_t messageSizeTMP = strlen(message);
	for(int i = 0; i< messageSizeTMP; i++){
		Serial.println(message[i],BIN);
	}	
	Serial.println(F("**[BUILDER][setToken]:END"));
}

//to simplyfy operating rising option adding order is required
//payload must be set after options too
void CoapBuilder::setOption(uint32_t optionNumber, char* value)
{
  _lastOptionLen = strlen(value);
  _lastOptionNum = optionNumber;
  uint8_t optNumOffset = 0;
  uint8_t optLenOffset = 0;
  

  if (message[_lastOptionStart] != '\0') //there was some option set
  {
    optionNumber -= _lastOptionNum;
    _lastOptionStart += _lastOptionLen;
	 Serial.println("tak");
  }
  
  //writing option number 
  if (optionNumber < 13) {
	  Serial.println(optionNumber);
	  Serial.println((optionNumber << 4),BIN);
	  Serial.println(optionNumber);
	  Serial.println(message[_lastOptionStart],BIN);
	  
	  
	  byte one = (optionNumber << 4);
	  char two = (optionNumber << 4);
	  uint8_t three = (optionNumber << 4);
	  
	  Serial.println(one,BIN);
	  Serial.println(two,BIN);
	  Serial.println(three,BIN);
	  
	  Serial.println(optionNumber);
	  
	  char cos = 192;
	  Serial.println(cos,BIN);
	  cos = 127;
	  Serial.println(cos,BIN);
	  cos = 128;
	  Serial.println(cos,BIN);
	  cos = 129;
	  Serial.println(cos,BIN);
	  
	  
	  
	  message[_lastOptionStart] = cos;
	  Serial.println(message[_lastOptionStart],BIN);
	  
	  //char cos2 = (12*16);
	  //message[_lastOptionStart] = cos2;
	  //Serial.println(message[_lastOptionStart],BIN);
	  
	  
	  Serial.println(message[_lastOptionStart],BIN);
	  Serial.println(message[4],BIN);
  } else if (optionNumber < 269){
      message[_lastOptionStart] = (13 << 4);
      message[_lastOptionStart + 1] = (optionNumber - 13);
      optNumOffset = 1;
  } else {
    message[_lastOptionStart] = 14 << 4;
    message[_lastOptionStart + 1] = (((optionNumber - 269) & 0xff00) >> 8);
    message[_lastOptionStart + 2] = ((optionNumber - 269) & 0x00ff);
    optNumOffset = 2;
  } 
  
    Serial.println(F("**[BUILDER][setOptions]:Option nubber"));
	Serial.println(_lastOptionStart);
	Serial.println(message[0],BIN);
	Serial.println(message[1],BIN);
	Serial.println(message[2],BIN);
	Serial.println(message[3],BIN);
	Serial.println(message[4],BIN);
	Serial.println(F("**[BUILDER][setOptions]:Option nubberEND"));
  
  
  //writing option length 
  if (_lastOptionLen < 13) {
      message[_lastOptionStart] += _lastOptionLen;
  } else if (_lastOptionLen < 269){
      message[_lastOptionStart] += 13;
      message[_lastOptionStart + 1 + optNumOffset] = _lastOptionLen - 13;
      optLenOffset = 1;
  } else {
    message[_lastOptionStart] += 14;
    message[_lastOptionStart + 1 + optNumOffset] = ((_lastOptionLen - 269) & 0xff00) >> 8;
    message[_lastOptionStart + 2 + optNumOffset] = (_lastOptionLen - 269) & 0x00ff;
    optLenOffset = 2;
  }
  
  
    Serial.println(F("**[BUILDER][setOptions]:Option length"));
	Serial.println(message[_lastOptionStart]);
	Serial.println(message[0],BIN);
	Serial.println(message[1],BIN);
	Serial.println(message[2],BIN);
	Serial.println(message[3],BIN);
	Serial.println(message[4],BIN);
	Serial.println(F("**[BUILDER][setOptions]:Option length"));
  
  
  //auxiliary adding to deacrease adding operations in for loop
  _lastOptionStart += optNumOffset + optLenOffset + 1;
  //writing the value of option
  for (uint8_t i = 0; i < _lastOptionLen; i++) {
    message[_lastOptionStart + i] = value[i]; 
  }
  //ending message
  message[_lastOptionStart + _lastOptionLen] = '\0';
  
  //redo autiliary adding
  _lastOptionStart -= optNumOffset + optLenOffset + 1;
  //adding option header to option value length - needed if many options are set
  _lastOptionLen += optNumOffset + optLenOffset + 1;
  
  
  	Serial.println(F("**[BUILDER][setOptions]:Hole Message"));	
	Serial.println(message[0],BIN);
	Serial.println(message[1],BIN);
	Serial.println(message[2],BIN);
	Serial.println(message[3],BIN);
	Serial.println(message[4],BIN);	
	Serial.println(F("**[BUILDER][setOptions]:END"));
}

//payload must be set after setting options
void CoapBuilder::setPayload(char* value)
{
  uint8_t i;
  uint8_t messageLen = strlen(message);
  //adding payload marker
  message[messageLen ++] = 255;
  for (i = 0; i < strlen(value); i++) {
    message[messageLen + i] = value[i];
  }
  message[messageLen + i] = '\0';
  _payloadLen = strlen(value);
  
  
//   Serial.println(F("**[BUILDER][setPayload1]:Hole Message"));	
//	size_t messageSizeTMP = strlen(message);
//	for(int i = 0; i< messageSizeTMP; i++){
//		Serial.println(message[i],BIN);
//	}	
//	Serial.println(F("**[BUILDER][setPayload1]:END"));
}

void CoapBuilder::setPayload(char* value, uint8_t start)
{
  uint8_t i;
  uint8_t messageLen = strlen(message);
  //adding payload marker
  message[messageLen ++] = 255;
  for (i = start; i < strlen(value); i++) {
    message[messageLen + i] = value[i];
  }
  message[messageLen + i] = '\0';
  _payloadLen = strlen(value);
  
//     Serial.println(F("**[BUILDER][setPayload2]:Hole Message"));	
//	size_t messageSizeTMP = strlen(message);
//	for(int i = 0; i< messageSizeTMP; i++){
//		Serial.println(message[i],BIN);
//	}	
//	Serial.println(F("**[BUILDER][setPayload2]:END"));
}

//append string to payload
void CoapBuilder::appendPayload(char* value) {
	uint8_t i;
  uint8_t messageLen = strlen(message);
  for (i = 0; i < strlen(value); i++) {
    message[messageLen + i] = value[i];
  }
  message[messageLen + i] = '\0';
  _payloadLen += strlen(value);
}

void CoapBuilder::appendPayload(char* value, uint8_t len) {
	uint8_t i;
  uint8_t messageLen = strlen(message);
  for (i = 0; i < len; i++) {
    message[messageLen + i] = value[i];
  }
  message[messageLen + i] = '\0';
  _payloadLen += strlen(value);
}

void CoapBuilder::appendPayload(char* value, uint8_t start, uint8_t end) {
	uint8_t i;
  uint8_t messageLen = strlen(message);
  for (i = start; i < end; i++) {
    message[messageLen + i] = value[i];
  }
  message[messageLen + i] = '\0';
  _payloadLen += strlen(value);
}
// remove Payload from message (payload tag is removed too)
void CoapBuilder::flushPayload() {
	_payloadLen = 0;
		uint8_t i;
  for (i = 0; i < strlen(message); i++) {
    if (message[i] == 255) {
		message[i] = '\0';
		return;
  }
}
}
// remove Payload from message (payload tag is removed too)
uint8_t CoapBuilder::getPayloadLen() {
	return _payloadLen;
}

char* CoapBuilder::build()
{
	
//	   Serial.println(F("**[BUILDER][build]:Hole Message"));	
//	size_t messageSizeTMP = strlen(message);
//	for(int i = 0; i< messageSizeTMP; i++){
//		Serial.println(message[i],BIN);
//	}	
//	Serial.println(F("**[BUILDER][build]:END"));
	
  return message;
}

