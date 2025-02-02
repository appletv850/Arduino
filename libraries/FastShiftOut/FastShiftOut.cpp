//
//    FILE: FastShiftOut.cpp
//  AUTHOR: Rob Tillaart
// VERSION: 0.2.5
// PURPOSE: ShiftOut that implements the Print interface
//    DATE: 2013-08-22
//     URL: https://github.com/RobTillaart/FastShiftOut
//
// HISTORY: see changelog.md


#include "FastShiftOut.h"


FastShiftOut::FastShiftOut(uint8_t dataOut, uint8_t clockPin, uint8_t bitOrder)
{
  _bitOrder = bitOrder;
  pinMode(dataOut, OUTPUT);
  pinMode(clockPin, OUTPUT);
  //  https://www.arduino.cc/reference/en/language/functions/advanced-io/shiftout/
  digitalWrite(clockPin, LOW);  //  assume rising pulses from clock

#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)

  uint8_t _port    = digitalPinToPort(dataOut);
  _dataOutRegister = portOutputRegister(_port);
  _dataOutBit      = digitalPinToBitMask(dataOut);

  _port            = digitalPinToPort(clockPin);
  _clockRegister   = portOutputRegister(_port);
  _clockBit        = digitalPinToBitMask(clockPin);

#else   //  reference implementation

  _dataPinOut = dataOut;
  _clockPin = clockPin;

#endif
}


size_t FastShiftOut::write(uint8_t data)
{
  _value = data;
  if (_bitOrder == LSBFIRST)
  {
    return writeLSBFIRST(data);
  }
  return writeMSBFIRST(data);
}


size_t FastShiftOut::writeLSBFIRST(uint8_t data)
{
  uint8_t value = data;
  _value = value;
  
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)

  uint8_t cbmask1  = _clockBit;
  uint8_t cbmask2  = ~_clockBit;
  uint8_t outmask1 = _dataOutBit;
  uint8_t outmask2 = ~_dataOutBit;

  for (uint8_t m = 1; m > 0; m <<= 1)
  {
    uint8_t oldSREG = SREG;
    noInterrupts();
    if ((value & m) == 0) *_dataOutRegister &= outmask2;
    else                  *_dataOutRegister |= outmask1;
    *_clockRegister |= cbmask1;
    *_clockRegister &= cbmask2;
    SREG = oldSREG;
  }

#else

  shiftOut(_dataPinOut, _clockPin, LSBFIRST, value);

#endif

  return 1;
}


size_t FastShiftOut::writeMSBFIRST(uint8_t data)
{
  uint8_t value = data;
  _value = value;
  
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)

  uint8_t cbmask1  = _clockBit;
  uint8_t cbmask2  = ~_clockBit;
  uint8_t outmask1 = _dataOutBit;
  uint8_t outmask2 = ~_dataOutBit;

  for (uint8_t m = 0x80; m > 0; m >>= 1)
  {
    uint8_t oldSREG = SREG;
    noInterrupts();
    if ((value & m) == 0) *_dataOutRegister &= outmask2;
    else                  *_dataOutRegister |= outmask1;
    *_clockRegister |= cbmask1;
    *_clockRegister &= cbmask2;
    SREG = oldSREG;
  }

#else

  shiftOut(_dataPinOut, _clockPin, MSBFIRST, value);

#endif

  return 1;

}


uint8_t FastShiftOut::lastWritten(void)
{
  return _value;
};


bool FastShiftOut::setBitOrder(const uint8_t bitOrder)
{
  if ((bitOrder == LSBFIRST) || (bitOrder == MSBFIRST))
  {
    _bitOrder = bitOrder;
    return true;
  };
  return false;
}


uint8_t FastShiftOut::getBitOrder(void)
{
  return _bitOrder;
};


// -- END OF FILE --

