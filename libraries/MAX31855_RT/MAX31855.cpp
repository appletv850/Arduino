//
//    FILE: MAX31855.cpp
//  AUTHOR: Rob Tillaart
// VERSION: 0.4.2
// PURPOSE: Arduino library for MAX31855 chip for K type thermocouple
//    DATE: 2014-01-01
//     URL: https://github.com/RobTillaart/MAX31855_RT
//
// HISTORY: see changelog.md


#include "MAX31855.h"


MAX31855::MAX31855()
{
}


void MAX31855::begin(const uint8_t select)
{
  begin(255, select, 255);
}


void MAX31855::begin(const uint8_t clock, const uint8_t select, const uint8_t miso)
{
  _clock        = clock;
  _miso         = miso;
  _select       = select;
  _hwSPI        = (_clock == 255);

  _lastTimeRead = 0;
  _offset       = 0;
  _SeebeckC     = K_TC;
  _status       = STATUS_NOREAD;
  _temperature  = MAX31855_NO_TEMPERATURE;
  _internal     = MAX31855_NO_TEMPERATURE;
  _rawData      = 0;
  setSPIspeed(1000000);

  pinMode(_select, OUTPUT);
  digitalWrite(_select, HIGH);

  if (_hwSPI)
  {
    #if defined(ESP32)
    if (_useHSPI)      //  HSPI
    {
      mySPI = new SPIClass(HSPI);
      mySPI->end();
      mySPI->begin(14, 12, 13, _select);   //  CLK=14  MISO=12  MOSI=13
    }
    else               //  VSPI
    {
      mySPI = new SPIClass(VSPI);
      mySPI->end();
      mySPI->begin(18, 19, 23, _select);   //  CLK=18  MISO=19  MOSI=23
    }
    #else              //  generic hardware SPI
    mySPI = &SPI;
    mySPI->end();
    mySPI->begin();
    #endif
    delay(1);
  }
  else
  {
    pinMode(_clock, OUTPUT);
    digitalWrite(_clock, LOW);
    pinMode(_miso, INPUT);
  }
}


void MAX31855::setSPIspeed(uint32_t speed)
{
  _SPIspeed = speed;
  _spi_settings = SPISettings(_SPIspeed, MSBFIRST, SPI_MODE0);
};


#if defined(ESP32)
void MAX31855::setGPIOpins(uint8_t clock, uint8_t miso, uint8_t mosi, uint8_t select)
{
  _clock   = clock;
  _miso    = miso;
  _select  = select;
  pinMode(_select, OUTPUT);
  digitalWrite(_select, HIGH);

  //  disable SPI and enable again
  mySPI->end();
  mySPI->begin(clock, miso, mosi, select);
}
#endif


uint8_t MAX31855::read()
{
  //  return value of _read()
  //  BITS        DESCRIPTION
  //  -------------------------------
  //  00 - 02     STATUS
  //       03     RESERVED
  //  04 - 15     INTERNAL
  //       16     FAULT-BIT
  //       17     RESERVED
  //  18 - 30     TEMPERATURE (RAW)
  //       31     SIGN
  uint32_t value = _read();

  if (value == 0xFFFFFFFF)  //  needs a pull up on MISO pin to work properly!
  {
    //  bit 3 and bit 17 should always be 0 - P10 datasheet
    _status = STATUS_NO_COMMUNICATION;
    return _status;
  }

  _lastTimeRead = millis();

  //  process status bit 0-2
  _status = value & 0x0007;
  // if (_status != STATUS_OK)  //  removed in 0.4.0 as internal can be valid.
  // {
  //   return _status;
  // }

  value >>= 3;

  //  reserved bit 3, always 0
  value >>= 1;

  //  process internal bit 4-15
  _internal = (value & 0x07FF) * 0.0625;
  //  negative flag set ?
  if (value & 0x0800)
  {
    _internal = -128 + _internal;
  }
  value >>= 12;

  //  Fault bit ignored as we have the 3 status bits
  //  _fault = value & 0x01;
  value >>= 1;

  //  reserved bit 17, always 0
  value >>= 1;

  //  process temperature bit 18-30 + sign bit = 31
  _temperature = (value & 0x1FFF) * 0.25;
  //  negative flag set ?
  if (value & 0x2000)
  {
    _temperature = -2048 + _temperature;
  }
  return _status;
}


uint32_t MAX31855::_read(void)
{
  _rawData = 0;
  //  DATA TRANSFER
  if (_hwSPI)
  {
    mySPI->beginTransaction(_spi_settings);
    digitalWrite(_select, LOW);         // must be after mySPI->beginTransaction() - see #14 STM32
    for (uint8_t i = 0; i < 4; i++)
    {
      _rawData <<= 8;
      _rawData += mySPI->transfer(0);
    }
    digitalWrite(_select, HIGH);
    mySPI->endTransaction();
  }
  else  //  Software SPI
  {
    //  split _swSPIdelay in equal dLow and dHigh
    //  dLow should be longer one when _swSPIdelay = odd.
    uint16_t dHigh = _swSPIdelay/2;
    uint16_t dLow = _swSPIdelay - dHigh;
    digitalWrite(_select, LOW);
    for (int8_t i = 31; i >= 0; i--)
    {
      _rawData <<= 1;
      digitalWrite(_clock, LOW);
      if (dLow > 0) delayMicroseconds(dLow);    //  DUE might need 1 us
      if ( digitalRead(_miso) ) _rawData++;
      digitalWrite(_clock, HIGH);
      if (dHigh > 0) delayMicroseconds(dHigh);  //  DUE
    }
    digitalWrite(_select, HIGH);
  }

  return _rawData;
}


float MAX31855::getTemperature()
{
  //  offset needs to be added after multiplication TCfactor
  //  not before otherwise offset will be larger / smaller
  //  default behaviour
  if (_SeebeckC == K_TC) return _temperature + _offset;

  //  EXPERIMENTAL OTHER THERMOCOUPLES
  //  to be tested
  //  in practice this works also for K_TC but is way slower..
  //  1: reverse calculate the Voltage measured  (is this correct?)
  float Vout = K_TC * (_temperature - _internal);  //  PAGE 8 datasheet

  //  2: from Voltage to corrected temperature using the Seebeck Coefficient
  float _temp = Vout / _SeebeckC + _internal + _offset;
  return _temp;
}


// -- END OF FILE --

