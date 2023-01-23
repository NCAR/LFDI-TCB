/*
 * This Document Consists of Two Tables
 * 	One To convert the Users request for nm Tuning to a voltage at 22C
 * 		ie if I wanted 656.28 nm that would be in tune at 3V at 22C
 * 	the Second is to get the Offset of the needed Voltage Depending on the current temperature
 *
 */

#ifndef INC_TEMPERATURECORRECTIONTABLE_H_
#define INC_TEMPERATURECORRECTIONTABLE_H_

#include "defs.h"



uint16_t NM_to_Position(float* nm);
float BaseT_Position_to_BaseT_Voltage(uint16_t position);
uint16_t temperature_position_offset(double* temp);
float Wavelength_to_Voltage(float* wavelength, double* temp);


#endif
