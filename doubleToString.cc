namespace FHDI {
	#include <string>
	#include <stdlib.h>
	#include <math.h> /* fmod */

	char* doubleToString(double num) {
		int count = 0, length = 0;

		// Calculates count of number after '.'
		while (fmod(num, 1)) != 0) {
		num *= 10;
		count++;
	}

	// Converts double to long
	long longNum = (long)num;

	// Calculates length of longNum
	long tmp = longNum;
	while (tmp != 0) {
		tmp /= 10;
		length++;
	}

	char longStr[length];
	_ltoa(longNum, longStr, 10); // converts long number to string
	char ret[length + 1]; // prepares string that includes '.'

	for (int i = 0; i < length + 1; i++) {
		if (i < length - count) {
			ret[i] = longStr[i] // copy numbers before '.'
		}
		else if (i == length - count) {
			ret[i] = '.';
		}
		else {
			ret[i] = longStr[i - 1];
		}
	}

	return ret;
	}
}