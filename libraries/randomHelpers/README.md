
[![Arduino CI](https://github.com/RobTillaart/randomHelpers/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![Arduino-lint](https://github.com/RobTillaart/randomHelpers/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/RobTillaart/randomHelpers/actions/workflows/arduino-lint.yml)
[![JSON check](https://github.com/RobTillaart/randomHelpers/actions/workflows/jsoncheck.yml/badge.svg)](https://github.com/RobTillaart/randomHelpers/actions/workflows/jsoncheck.yml)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/RobTillaart/randomHelpers/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/RobTillaart/randomHelpers.svg?maxAge=3600)](https://github.com/RobTillaart/randomHelpers/releases)


# randomHelpers

Arduino library for faster generation of random numbers.


## Description

This library contains functions that have the goal to deliver random bits faster
than the build in random function can, while still using it.

The idea is to have a buffer ( __randomBuffer) which can hold up to 32 bits.
When a number of random bits are needed, these are first fetched from the 
buffer and if the buffer gets empty, it is filled again with a call to the
random generator.

This strategy works well with a 32 bits buffer and requests for 1..16 random bits. 
However above 16 bits the overhead is larger than the gain. 
So to improve in that range too one could use a faster random function like the one 
from Marsaglia (included).

Note the gains differ per platform and are more explicit on the (slow) Arduino UNO 
platform than on a much faster ESP32. 

This library relates to https://github.com/RobTillaart/Prandom


## Interface

#### generators (sort of)

- **uint32_t Marsaglia()** fast PRNG.
- **bool seedMarsaglia(uint32_t a, uint32_t b)** seed the Marsaglia PRNG. a and b should not be 0. returns true on success.

#### getters (sort of)
- **bool getRandom1()** returns 0 or 1, false or true. 
- **uint8_t getRandom4()** returns 0 .. 15.
- **uint8_t getRandom5()** returns 0 .. 31.
- **uint8_t getRandom6()** returns 0 .. 63.
- **uint8_t getRandom8()** returns 0 .. 255 typically a byte.
- **uint16_t getRandom16()** returns 0 .. 65535 (2 bytes).
- **uint32_t getRandom24()** returns 0 .. 16777215  (3 bytes), e.g. random RGB colour.
- **uint32_t getRandom32()** returns 0 .. 2^32 - 1 (4 bytes) this is the core random generator
- **uint64_t getRandom64()** returns 0.. 2^64 - 1 (8 bytes).
- **uint32_t getRandomBits(n)** returns 0.. 2^n - 1  This works well for 1..16 bits but above 16 it is slower than the standard way. 

#### Typical wrappers.

- **bool flipCoin()** A wrapper around getRandom1().
- **uint8_t throwDice()** returns 1..6.

The examples show how to use these and how their performance gain relative to
calling **random()** for every random number.

## Performance

to elaborate

## Operation

See examples


## Future

#### must
- improve/update documentation
  - add performance figures

#### should
- wrap all up in a class.
- add JKISS? other RNG's

#### could
- improve performance getRandomBits(n) for n = 17..31
- investigate new tricks :)
- test if the functions are uniform.
- rename getRandom64() ==> get64()  etc.
  - when it is part of a class.
- add **getRandom2(), getRandom3(), getRandom12()**?


