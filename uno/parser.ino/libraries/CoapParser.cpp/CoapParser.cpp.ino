#include "Arduino.h"
#include "CoapParser.h"

CoapParser::CoapParser(){}

//Poprawic fukcje - przesuniecie jeszcze dodac!

//tylko wartosc 1 poprawna
int CoapParser::parseVersion(char* message)
{
  return (message[0] & 0xb0);
  //reply[0] = reply[0] & 0xb0;
  //reply[0] = reply[0] + (value & 0x03) ;
}

uint8_t CoapParser::parseType(char* message)
{
  return (message[0] & 0x30);
}

//tylko wartosci 0-8 poprawne
uint8_t CoapParser::parseTokenLen(char* message)
{
  return (message[0] & 0x0f);
}

//tylko wartosci {0,2,4,5} poprawne
uint8_t CoapParser::parseCodeClass(char* message)
{
  return (message[1] & 0xe0);
}

uint8_t CoapParser::parseCodeDetail(char* message)
{
  return (message[1] & 0x1f);
}

uint16_t CoapParser::parseMessageId(char* message)
{
  return (message[2]& 0xff) << 8  + message[3];
}

char* CoapParser::parseToken(char* message, uint8_t tokenLen) 
{
  uint8_t byteNumber;
   for (byteNumber = 0; byteNumber < tokenLen; byteNumber++) {
      fieldValue[byteNumber] = message[4+byteNumber];
   }
   fieldValue[byteNumber] = '\0';
   return fieldValue;
}

void CoapParser::parseOptions(char* message)
{
  _optionsCount = 0;
  int currentOptionStart = 4 + parseTokenLen(message);
  while (message[currentOptionStart] != 0xff) {
    _optionsCount++;
    
    currentOptionStart += message[currentOptionStart]
  }
}
//0 - brak opcji
uint8_t CoapParser::getFirstOptionType() {
  _currentOptionNumber = 4 + _tokenLen;
  if (message[currentOptionStart] == 0xff) {
    return 0;
  }
  else {
    
  }
}

uint8_t CoapParser::getFirstOptionValue() {
  
  
}

uint8_t CoapParser::getNextOption() {
  
}

