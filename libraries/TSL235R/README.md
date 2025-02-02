
[![Arduino CI](https://github.com/RobTillaart/TSL235R/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![Arduino-lint](https://github.com/RobTillaart/TSL235R/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/RobTillaart/TSL235R/actions/workflows/arduino-lint.yml)
[![JSON check](https://github.com/RobTillaart/TSL235R/actions/workflows/jsoncheck.yml/badge.svg)](https://github.com/RobTillaart/TSL235R/actions/workflows/jsoncheck.yml)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/RobTillaart/TSL235R/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/RobTillaart/TSL235R.svg?maxAge=3600)](https://github.com/RobTillaart/TSL235R/releases)


# TSL235R

Arduino library for the TSL235R light to frequency convertor.


## Description

This library does not measure the frequency.
That should be done with an interrupt function (user code).
The library provides functions to calculate the radiation and 
has functions to compensate e.g. for wavelength and voltage used.

The library is not tested extensively yet.

The sensor operating voltage is between 2.7 and 5.5 max.
This can be set in the constructor, or runtime adjusted.

For measurements below 1 uW/cm2 one should measure for multiple seconds.
Above 1 uW/cm2 1 second or shorter is OK. 

Note that for longer and shorter measurements than 1 second one must 
convert the value to Hz, which is the number of pulses in 1 second.
The library provides functions **irradiance()** to do that for you.


## Connection

Always check the datasheet.

```
//  PIN 1  -  GND
//  PIN 2  -  VDD      2.7 V .. 5.5 V
//  PIN 3  -  SIGNAL   1 Hz  .. 800 KHz
```


## Interface

- **TSL235R(float voltage = 5.0)** constructor, optionally one can give the operational voltage 
to add a small correction (< 1.5%).
Default voltage is 5.0 Volts.
- **float irradiance(uint32_t Hz)** returns the irradiance in uW/cm2.
Note that Hz implies the measured pulses for 1 second.
- **float irradiance(uint32_t pulses, uint32_t milliseconds)** returns the irradiance in uW/cm2.
This formula is used for other duration than 1 second.
To get irradiance in W/m2 one must divide by 100.
- **float irradiance_HS(uint32_t pulses, uint32_t microseconds)** returns the irradiance in uW/cm2.
This formula is used when the time is measured in microseconds. 
This is the most accurate measurement.
- **float getFactor()** returns the inner conversion factor from Hz to Watt/cm2.
- **void setWavelength(uint16_t wavelength = 635)** sets the wavelength so the formulas can use a 
correction factor. 
At the default wavelength of 635 nm the wavelength correction factor == 1.0.
- **uint16_t getWavelength()** returns the set wavelength.
- **float getWaveLengthFactor()** returns the wavelength correction factor. 
As the sensor is most sensitive around 750 nm this value helps to normalize the signal.
This works only for (almost) monochromatic light.
- **void setVoltage(float voltage)** sets the voltage so the formulas can use a correction factor.
This voltage correction factor is rather small < 1.5%.
Note: this voltage can be changed runtime.
- **float getVoltage()** returns the set voltage, by constructor or by **setVoltage()**.
Default voltage is 5.0 Volts.


## Operations

See examples for typical usage.


## Future

#### must
- improve documentation
- test test test

#### should
- default voltage should be a #define so people who use 3.3V processor
  can change the library with minimal effort.
- irradiance(pulses, millis) can be given a default of 1000 millis.
  makes irradiance(Hz) obsolete. 
- move code from .h to .cpp

#### could
- investigate hardware solutions for e.g. divide by 100 or 1000 or so.
- investigate correction factor for white light and mixed light sources.
- investigate calibration factor for timing of processor used.

