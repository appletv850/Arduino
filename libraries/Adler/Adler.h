#pragma once
//
//    FILE: Adler.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.2.2
//    DATE: 2022-01-27
// PURPOSE: Arduino Library for calculating Adler checksum
//     URL: https://github.com/RobTillaart/Adler
//          https://en.wikipedia.org/wiki/Adler-32


#include "Arduino.h"


#define ADLER_LIB_VERSION        (F("0.2.2"))


const uint32_t ADLER32_MOD_PRIME = 65521;
const uint16_t ADLER16_MOD_PRIME = 251;


/////////////////////////////////////////////////
//
//  STATIC FUNCTIONS
//
uint32_t adler32(uint8_t *data, uint16_t length);
uint16_t adler16(uint8_t *data, uint16_t length);


// -- END OF FILE -- 


