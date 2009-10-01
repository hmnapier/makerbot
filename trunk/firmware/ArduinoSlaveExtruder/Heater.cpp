#include "Heater.h"
#include "Variables.h"
#include "Configuration.h"
#include "ThermistorTable.h"

void set_temperature(int temp)
{
  target_temperature = temp;
  max_temperature = (int)((float)temp * 1.1);
}

#ifdef THERMISTOR_PIN
int read_thermistor();
#endif // THERMISTOR_PIN

#ifdef THERMOCOUPLE_PIN
int read_thermocouple();
#endif // THERMOCOUPLE_PIN

/**
 *  Samples the temperature and converts it to degrees celsius.
 *  Returns degrees celsius.
 */
int get_temperature()
{
#ifdef THERMISTOR_PIN
  return read_thermistor();
#endif
#ifdef THERMOCOUPLE_PIN
  return read_thermocouple();
#endif
}

/*
* This function gives us the temperature from the thermistor in Celsius
 */
#ifdef THERMISTOR_PIN
int read_thermistor()
{
  int raw = sample_temperature(THERMISTOR_PIN);

  int celsius = 0;
  byte i;

  for (i=1; i<NUMTEMPS; i++)
  {
    if (temptable[i][0] > raw)
    {
      celsius  = temptable[i-1][1] + 
        (raw - temptable[i-1][0]) * 
        (temptable[i][1] - temptable[i-1][1]) /
        (temptable[i][0] - temptable[i-1][0]);

      if (celsius > 255)
        celsius = 255; 

      break;
    }
  }

  // Overflow: We just clamp to 0 degrees celsius
  if (i == NUMTEMPS)
    celsius = 0;

  return celsius;
}
#endif

/*
* This function gives us the temperature from the thermocouple in Celsius
 */
#ifdef THERMOCOUPLE_PIN
int read_thermocouple()
{
  return ( 5.0 * sample_temperature(THERMOCOUPLE_PIN) * 100.0) / 1024.0;
}
#endif

/*
* This function gives us an averaged sample of the analog temperature pin.
 */
int sample_temperature(uint8_t pin)
{
  int raw = 0;

  //read in a certain number of samples
  for (byte i=0; i<TEMPERATURE_SAMPLES; i++)
    raw += analogRead(pin);

  //average the samples
  raw = raw/TEMPERATURE_SAMPLES;

  //send it back.
  return raw;
}


/*!
 Manages motor and heater based on measured temperature:
 o If temp is too low, don't start the motor
 o Adjust the heater power to keep the temperature at the target
 */
void manage_temperature()
{
  //make sure we know what our temp is.
  current_temperature = get_temperature();

  //put the heater into high mode if we're not at our target.
  if (current_temperature < target_temperature)
    analogWrite(HEATER_PIN, heater_high);
  //put the heater on low if we're at our target.
  else if (current_temperature < max_temperature)
    analogWrite(HEATER_PIN, heater_low);
  //turn the heater off if we're above our max.
  else
    analogWrite(HEATER_PIN, 0);
}

