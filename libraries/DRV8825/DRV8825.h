#pragma once
//
//    FILE: DRV8825.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.3
// PURPOSE: Arduino library for DRV8825 stepper motor driver
//    DATE: 2022-07-07
//     URL: https://github.com/RobTillaart/DRV8825


#include "Arduino.h"


#define DRV8825_LIB_VERSION              (F("0.1.3"))


//  setDirection
const uint8_t DRV8825_CLOCK_WISE        = 0;  //  LOW
const uint8_t DRV8825_COUNTERCLOCK_WISE = 1;  //  HIGHT

class DRV8825
{
public:
  DRV8825();

  bool     begin(uint8_t DIR, uint8_t STEP, uint8_t EN = 255, uint8_t RST = 255, uint8_t SLP = 255);

  //       DIRECTION
  //       0 = DRV8825_CLOCK_WISE
  //       1 = DRV8825_COUNTERCLOCK_WISE
  //       returns false if parameter out of range.
  bool     setDirection(uint8_t direction = DRV8825_CLOCK_WISE);
  uint8_t  getDirection();

  //       STEPS
  void     setStepsPerRotation(uint16_t stepsPerRotation);
  uint16_t getStepsPerRotation();
  void     step();
  uint32_t resetSteps(uint32_t s = 0 );
  uint32_t getSteps();

  //       POSITION
  //       only works if stepsPerRotation > 0
  //       returns false if pos > stepsPerRotation.
  bool     setPosition(uint16_t pos = 0);
  uint16_t getPosition();

  //       CONFIGURATION
  //       step pulse length is in microseconds
  //       datasheet default = 1.9 us
  void     setStepPulseLength(uint16_t us = 2);
  uint16_t getStepPulseLength();

  //       ENABLE pin should be set.
  void     enable();
  void     disable();
  bool     isEnabled();

  //       RESET pin should be set.
  void     reset();

  //       SLEEP pin should be set.
  void     sleep();
  void     wakeup();
  bool     isSleeping();


protected:
  uint8_t  _directionPin     = 255;
  uint8_t  _stepPin          = 255;
  uint8_t  _enablePin        = 255;
  uint8_t  _resetPin         = 255;
  uint8_t  _sleepPin         = 255;

  uint8_t  _direction        = DRV8825_CLOCK_WISE;

  uint16_t _stepsPerRotation = 0;
  uint32_t _steps            = 0;
  uint16_t _pos              = 0;
  uint16_t _us               = 2;
};


//  -- END OF FILE --


