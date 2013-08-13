#include "HelperFunctions.h"

// Source: http://stackoverflow.com/questions/3407012/c-rounding-up-to-the-nearest-multiple-of-a-number
int RoundUp(int numToRound, int multiple) 
{ 
        if(multiple == 0) 
        { 
                return numToRound; 
        } 

        int remainder = numToRound % multiple;

        if (remainder == 0)
        {
                return numToRound;
        }

        return numToRound + multiple - remainder;
} 

bool IsInt(char character)
{
	int value = character - '0';

	if(value >= 0 && value <= 9)
	{
		return true;
	}

	return false;
}