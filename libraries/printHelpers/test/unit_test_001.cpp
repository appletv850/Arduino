//
//    FILE: unit_test_001.cpp
//  AUTHOR: Rob Tillaart
//    DATE: 2021-01-06
// PURPOSE: unit tests for the printHelpers
//          https://github.com/RobTillaart/printHelpers
//          https://github.com/Arduino-CI/arduino_ci/blob/master/REFERENCE.md
//

// supported assertions
// ----------------------------
// assertEqual(expected, actual);               // a == b
// assertNotEqual(unwanted, actual);            // a != b
// assertComparativeEquivalent(expected, actual);    // abs(a - b) == 0 or (!(a > b) && !(a < b))
// assertComparativeNotEquivalent(unwanted, actual); // abs(a - b) > 0  or ((a > b) || (a < b))
// assertLess(upperBound, actual);              // a < b
// assertMore(lowerBound, actual);              // a > b
// assertLessOrEqual(upperBound, actual);       // a <= b
// assertMoreOrEqual(lowerBound, actual);       // a >= b
// assertTrue(actual);
// assertFalse(actual);
// assertNull(actual);

// // special cases for floats
// assertEqualFloat(expected, actual, epsilon);    // fabs(a - b) <= epsilon
// assertNotEqualFloat(unwanted, actual, epsilon); // fabs(a - b) >= epsilon
// assertInfinity(actual);                         // isinf(a)
// assertNotInfinity(actual);                      // !isinf(a)
// assertNAN(arg);                                 // isnan(a)
// assertNotNAN(arg);                              // !isnan(a)

#include <ArduinoUnitTests.h>


#include "Arduino.h"
#include "printHelpers.h"


unittest_setup()
{
  fprintf(stderr, "PRINTHELPERS_VERSION: %s\n", (char *) PRINTHELPERS_VERSION);
}


unittest_teardown()
{
}


unittest(test_constants)
{
  assertEqual(PRINTBUFFERSIZE, 66);
}


unittest(test_sci)
{
  fprintf(stderr, "%s\n", sci(PI * 1000, 6));
  fprintf(stderr, "%s\n", sci(PI * 100, 6));
  fprintf(stderr, "%s\n", sci(PI * 10, 6));
  fprintf(stderr, "%s\n", sci(EULER * 1000, 6));
  fprintf(stderr, "%s\n", sci(EULER * 100, 6));
  fprintf(stderr, "%s\n", sci(EULER * 10, 6));
  fprintf(stderr, "\n");

  assertEqual(0, strcmp("3.141593E+1", sci(PI * 10, 6)) );
  assertEqual(0, strcmp("2.718282E+1", sci(EULER * 10, 6)) );
}


unittest(test_eng)
{
  int32_t value32 = 1UL << 25;
  
  fprintf(stderr, "VALUE32 = %d\n", value32);
  fprintf(stderr, "%s\n", eng(PI * 1000, 6));
  fprintf(stderr, "%s\n", eng(PI * 100, 6));
  fprintf(stderr, "%s\n", eng(PI * 10, 6));
  fprintf(stderr, "%s\n", eng(EULER * 1000, 6));
  fprintf(stderr, "%s\n", eng(EULER * 100, 6));
  fprintf(stderr, "%s\n", eng(EULER * 10, 6));
  fprintf(stderr, "\n");

  assertEqual(0, strcmp("3.141593E+3", eng(PI * 1000, 6)) );
  assertEqual(0, strcmp("27.182818E+0", eng(EULER * 10, 6)) );
}


unittest(test_print64)
{
  int64_t value64 = 1ULL << 35;

  fprintf(stderr, "%ld\n", value64);
  fprintf(stderr, "%s\n", print64(value64));
  fprintf(stderr, "%s\n", print64(value64, HEX));

  assertEqual(0, strcmp("34359738368", print64(value64)) );
  assertEqual(0, strcmp("0x800000000", print64(value64, HEX)) );
}


unittest(test_toBytes)
{
  for (int i = 0; i < 30; i++)
  {
    uint32_t t = random(pow(2, i));
    fprintf(stderr, "%d\t%s\n", t, toBytes(t, 3));
  }

  fprintf(stderr, "%.3f\n", 12345678 / 1024.0 / 1024.0);
  assertEqual(0, strcmp("11.773 MB", toBytes(12345678, 3)) );
}


unittest(test_hex)
{
  for (int i = 0; i < 30; i++)
  {
    uint32_t t = random(pow(2, i));
    fprintf(stderr, "%d\t%s\n", t, hex(t));
  }
  assertEqual(8, strlen(hex((uint32_t)33615)) );
  assertEqual(0, strcmp("0000834F", hex((uint32_t)33615)) );
}


unittest(test_bin)
{
  for (int i = 0; i < 30; i++)
  {
    uint32_t t = random(pow(2, i));
    fprintf(stderr, "%d\t%s\n", t, bin(t));
    assertEqual(32, strlen(bin(t)) );
  }
  assertEqual(32, strlen(bin((uint32_t)197493099)) );
  assertEqual(0, strcmp("00001011110001011000000101101011", bin((uint32_t)197493099)) );
}

unittest_main()

// --------
