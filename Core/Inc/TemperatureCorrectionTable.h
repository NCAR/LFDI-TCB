/*
 * This Document Consists of Two Tables
 * 	One To convert the Users request for nm Tuning to a voltage at 22C
 * 		ie if I wanted 656.28 nm that would be in tune at 3V at 22C
 * 	the Second is to get the Offset of the needed Voltage Depending on the current temperature
 *
 */


//This Will translate a nm Tuning request to a position at 25C
//The Range Should Cover 600nm to 700nm in 0.01 nm intervals or 10000 entries
uint16_t NM_to_Position(float nm)
{
//Index starts at 600 and moves to 700 in .01 intervals
//Convert the User input by subtracting 600 and muliplying by 10000
uint16 adjusted_input = (nm - 600)*10000;
static const __flash uint16_t lookup[10000] = {0};
pos = lookup[nm];
return ps;
}


//Lookup Table to convert a pixel position to a voltage at the base Temperature. 25C
uint16 BaseT_Position_to_BaseT_Voltage(uint16 position){
	static const __flash uint16_t lookup[15000] =
	{0
	};
	volts = lookup[volts];
	return volts;
}

//This Function Will Convert the temperature to a positional offset
//This is based on the Found relation ship between Position and Temperature
uint16_t temperature_position_offset(float* temp){
	offset = (uint16_t)(43.06*(&temp)-985.5);
	return offset;
}
