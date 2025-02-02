#pragma once
//
//    FILE: temperature.h
// VERSION: 0.3.5
//    DATE: 2015-03-29
// PURPOSE: collection temperature functions


#include "Arduino.h"


#define TEMPERATURE_VERSION         (F("0.3.5"))


float Fahrenheit(float celsius);

float Celsius(float Fahrenheit);

float Kelvin(float celsius);


//  reference:
//  [1] https://wahiduddin.net/calc/density_algorithms.htm
//  [2] https://web.archive.org/web/20100528030817/https://www.colorado.edu/geography/weather_station/Geog_site/about.htm
//  dewPoint function based on code of [2]
//  calculation of the saturation vapour pressure part is based upon NOAA ESGG(temp)
float dewPoint(float celsius, float humidity);



//  dewPointFast() is > 5x faster than dewPoint() - run dewpoint_test.ino
//  delta mdewPointFastax with dewPoint() - run dewpoint_test.ino ==> ~0.347
//  (earlier version mentions ~0.6544 but that test code is gone :(
//  http://en.wikipedia.org/wiki/Dew_point
float dewPointFast(float celsius, float humidity);



// https://en.wikipedia.org/wiki/Humidex
float humidex(float celsius, float dewPoint);



//  0.3.0 => https://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml 
//           previous  https://en.wikipedia.org/wiki/Heat_index
//  TF = temp in Fahrenheit
//  RH = relative humidity in %
float heatIndex(float TF, float RH);



//  0.3.0 => https://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml 
//           previous  https://en.wikipedia.org/wiki/Heat_index
//  TC = temp in Celsius
//  RH = relative humidity in %
float heatIndexC(float TC, float RH);



//  https://en.wikipedia.org/wiki/Wind_chill
//     US     = Fahrenheit / miles / hour
//     METRIC = Celsius    / meter / hour (sec)
//  wind speed @ 10 meter,
//  if convert is true => wind speed will be converted to 1.5 meter
//  else ==> formula assumes wind speed @ 1.5 meter


//  US
float WindChill_F_mph(const float Fahrenheit, const float milesPerHour, const bool convert = true);

//  METRIC - standard wind chill formula for Environment Canada
float WindChill_C_kmph(const float Celsius, const float kilometerPerHour, const bool convert = true);

float WindChill_C_mps(const float Celsius, const float meterPerSecond, const bool convert = true);


//  https://www.engineeringtoolbox.com/air-altitude-pressure-d_462.html
//    Does not have the temperature correction ==> it has almost the -5.257 exponent
//  https://www.omnicalculator.com/physics/air-pressure-at-altitude
//    similar to https://en.wikipedia.org/wiki/Barometric_formula
//
//  Note: altitude in meters.
float baroToSeaLevelC( float pressure, float celsius, float altitude);

//  https://www.omnicalculator.com/physics/air-pressure-at-altitude
//    temperature (Celsius) at altitude (meter)
float seaLevelToAltitude( float pressureSeaLevel, float celsius, float altitude);

float altitudeToSeaLevel( float pressure, float celsius, float altitude);



/////////////////////////////////////////////////////////////
//
//  TEMPERATURE CONVERTER CLASS
//
class temperatureConverter
{
  //  used Celsius as internal unit, to minimize math
  public:
    temperatureConverter()              { _temp = 0; };

    void setKelvin(float value = 0)     { _temp = value - 273.15; };
    void setCelsius(float value = 0)    { _temp = value; };
    void setFahrenheit(float value = 0) { _temp = (value - 32.0) / 1.8; };
    void setReamur(float value = 0)     { _temp = value * 1.25; };
    void setRankine(float value = 0)    { _temp = (value - 491.67) / 1.8; };
    void setDelisle(float value = 0)    { _temp = (value + 100) / 1.5; };
    void setNewton(float value = 0)     { _temp = value / 0.33; };
    void setRomer(float value = 0)      { _temp = (value - 7.5) / 0.525; };

    float getKelvin()      { return _temp + 273.15; };
    float getCelsius()     { return _temp; };
    float getFahrenheit()  { return _temp * 1.8 + 32; };
    float getReamur()      { return _temp * 0.8; };
    float getRankine()     { return _temp * 1.8 + 491.67; };
    float getDelisle()     { return _temp * 1.5 - 100.0; };
    float getNewton()      { return _temp * 0.33; };
    float getRomer()       { return _temp * 0.525 + 7.5; };

  private:
    float _temp = 0;
};


//  -- END OF FILE --

