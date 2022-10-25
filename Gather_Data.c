// --------------------------------------------------------------------------------
// Name: David Simpson
// Class: SET 151-400
// Abstract: Final Project - Gather Data
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Includes � built-in libraries of functions
// --------------------------------------------------------------------------------
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

// --------------------------------------------------------------------------------
// Constants
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// User Defined Types (UDT)
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Prototypes
// --------------------------------------------------------------------------------
void GetCensusData(char strDate[], char strState[], char strCountyOhio[], char strCountyKentucky[], char strRace[], int *pintHouseHeadCount, float *pfltHouseIncome);
void BuildDataString(char strFinal[], char strDate[], char strState[], char strCountyOhio[], char strCountyKentucky[], char strRace[], int intHouseHeadCount, float fltHouseIncome);
void WriteCensusData(char strFinal[]);

int ValidDate(char strDate[]);
int ValidChoiceRange(char strChoice[], int intRange);

void Concatenate(char strDestination[], char strSource[]);
int GetStringLength(char strSource[]);
void SliceString(char strDestination[], char strSource[], int intStartIndex, int intLength);

// --------------------------------------------------------------------------------
// Name: main
// Abstract: This is where the program starts
// --------------------------------------------------------------------------------
void main()
{
	// must be in format mm/dd/yyyy
	char strDate[11] = "";
	// 1. Ohio, 2. Kentucky
	char strState[2] = "";
	// County choices
	// Ohio: 1. Hamilton, 2. Butler
	char strCountyOhio[2] = "";
	strCountyOhio[0] = '0';
	// Kentucky: 1. Boone, 2. Kenton
	char strCountyKentucky[2] = "";
	strCountyKentucky[0] = '0';
	// race, 1. Caucasian, 2. African American, 3. Hispanic, 4. Asian, 5. Other
	char strRace[2] = "";
	// number of people in household
	int intHouseHeadCount = 0;
	// household income
	float fltHouseIncome = 0.0f;
	// final data string, max size for a record is 36, plus newline, plus null terminator
	char strDataFinal[38] = "";
	
	// exit variable
	char chrContinue = 'y';

	while (chrContinue == 'y' || chrContinue == 'Y')
	{
		GetCensusData(strDate, strState, strCountyOhio, strCountyKentucky, strRace, &intHouseHeadCount, &fltHouseIncome);
		BuildDataString(strDataFinal, strDate, strState, strCountyOhio, strCountyKentucky, strRace, intHouseHeadCount, fltHouseIncome);
		WriteCensusData(strDataFinal);

		printf("Do you want to enter another record (y/n)?");
		scanf(" %c", &chrContinue);
	}

	system("pause");
}

// --------------------------------------------------------------------------------
// Name: GetCensusData
// Abstract: Gets census data from user
// --------------------------------------------------------------------------------
void GetCensusData(char strDate[], char strState[], char strCountyOhio[], char strCountyKentucky[], char strRace[], int *pintHouseHeadCount, float *pfltHouseIncome)
{
	do
	{
		printf("Please enter a date, must be in format mm/dd/yyyy: ");
		scanf("%10s", strDate);
	} while (!ValidDate(strDate));

	do
	{
		printf("Please enter the state, 1 for Ohio or 2 for Kentucky: ");
		scanf("%1s", strState);
	} while (!ValidChoiceRange(strState, 2));

	switch (strState[0])
	{
	case '1':
		do
		{
			printf("Please enter the County, 1 for Hamilton or 2 for Butler: ");
			scanf("%1s", strCountyOhio);
		} while (!ValidChoiceRange(strCountyOhio, 2));
		break;
	case '2':
		do
		{
			printf("Please enter the County, 1 for Boone or 2 for Kenton: ");
			scanf("%1s", strCountyKentucky);
		} while (!ValidChoiceRange(strCountyKentucky, 2));
	}

	do
	{
		printf("Please choose a race:\n1. Caucasian\n2. African American\n3. Hispanic\n4. Asian\n5. Other\n");
		scanf("%1s", strRace);
	} while (!ValidChoiceRange(strRace, 5));

	// household head count shouldn't be less than 1 or greater than 99
	while (*pintHouseHeadCount < 1 || *pintHouseHeadCount > 99)
	{
		printf("Enter number of people in household: ");
		scanf("%d", pintHouseHeadCount);
	}

	// don't let income be less than or equal to 0, everyone makes something
	// also, we don't have any trillionaires in the world <yet>,
	// so income doesn't need to be greater than $999,999,999,999
	while (*pfltHouseIncome <= 0 || *pfltHouseIncome >= 1e12)
	{
		printf("Enter household income: ");
		scanf("%f", pfltHouseIncome);
	}
}

// --------------------------------------------------------------------------------
// Name: BuildDataString
// Abstract: Builds string that will be written to file
// --------------------------------------------------------------------------------
void BuildDataString(char strFinal[], char strDate[], char strState[], char strCountyOhio[], char strCountyKentucky[], char strRace[], int intHouseHeadCount, float fltHouseIncome)
{
	char strDelimiter[2] = ",";

	char strHouseHeadCount[3] = "";
	sprintf(strHouseHeadCount, "%d", intHouseHeadCount);
	char strHouseIncome[16] = "";
	sprintf(strHouseIncome, "%.2f", fltHouseIncome);

	Concatenate(strFinal, strDate);
	Concatenate(strFinal, strDelimiter);
	Concatenate(strFinal, strState);
	Concatenate(strFinal, strDelimiter);
	Concatenate(strFinal, strCountyOhio);
	Concatenate(strFinal, strDelimiter);
	Concatenate(strFinal, strCountyKentucky);
	Concatenate(strFinal, strDelimiter);
	Concatenate(strFinal, strRace);
	Concatenate(strFinal, strDelimiter);
	Concatenate(strFinal, strHouseHeadCount);
	Concatenate(strFinal, strDelimiter);
	Concatenate(strFinal, strHouseIncome);
	Concatenate(strFinal, "\n");
}

// --------------------------------------------------------------------------------
// Name: WriteCensusData
// Abstract: Writes data to file
// --------------------------------------------------------------------------------
void WriteCensusData(char strFinal[])
{
	char strFileName[20] = "res/census_data.txt";
	FILE *pfilCensusData = fopen(strFileName, "a");
	fputs(strFinal, pfilCensusData);
	fclose(pfilCensusData);
}

// --------------------------------------------------------------------------------
// Name: ValidDate
// Abstract: Checks if date is valid, in format mm/dd/yyyy
// --------------------------------------------------------------------------------
int ValidDate(char strDate[])
{
	// check for correct date string length
	if (GetStringLength(strDate) != 10)
	{
		return 0;
	}

	// check for correct locations of forward slashes
	if (strDate[2] != '/' || strDate[5] != '/')
	{
		return 0;
	}

	char strMonth[3];
	char strDay[3];
	char strYear[5];

	SliceString(strMonth, strDate, 0, 2);
	SliceString(strDay, strDate, 3, 2);
	SliceString(strYear, strDate, 6, 4);

	// make sure month is all numbers
	if (!atoi(strMonth))
	{
		return 0;
	}

	// make sure day is all numbers
	if (!atoi(strDay))
	{
		return 0;
	}

	// make sure year is all numbers
	if (!atoi(strYear))
	{
		return 0;
	}

	int intMonth = atoi(strMonth);
	int intDay = atoi(strDay);
	int intYear = atoi(strYear);

	// check year range
	if (intYear < 1 || intYear > 9999)
	{
		return 0;
	}

	// check if year is a leap year
	int intIsLeapYear = 0;
	if (intYear % 400 == 0)
	{
		intIsLeapYear = 1;
	}
	else if (intYear % 4 == 0 && intYear % 100 != 0)
	{
		intIsLeapYear = 1;
	}

	// check month range
	if (intMonth < 1 || intMonth > 12)
	{
		return 0;
	}

	// set max day
	int intMaxDay;
	switch (intMonth)
	{
	case 2:
		intMaxDay = 28;
		if (intIsLeapYear)
		{
			intMaxDay += 1;
		}
		break;
	case 4:
		intMaxDay = 30;
		break;
	case 6:
		intMaxDay = 30;
		break;
	case 9:
		intMaxDay = 30;
		break;
	case 11:
		intMaxDay = 30;
		break;
	default:
		intMaxDay = 31;
	}

	// check day range
	if (intDay < 1 || intDay > intMaxDay)
	{
		return 0;
	}

	// if we've reached this point, it's a good date
	return 1;
}

// --------------------------------------------------------------------------------
// Name: ValidChoiceRange
// Abstract: Validates selection from 1 to passed range, inclusive
// --------------------------------------------------------------------------------
int ValidChoiceRange(char strChoice[], int intRange)
{
	// make sure choice is all numbers
	if (!atoi(strChoice))
	{
		return 0;
	}

	int intChoice = atoi(strChoice);

	// test choice range
	if (intChoice >= 1 && intChoice <= intRange)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// --------------------------------------------------------------------------------
// Name: Concatenate
// Abstract: Appends source to end of destination string
// --------------------------------------------------------------------------------
void Concatenate(char strDestination[], char strSource[])
{
	// starting index is the end of the destination string
	int intIndex = GetStringLength(strDestination);
	int intSourceIndex = 0;

	while (strSource[intSourceIndex] != 0)
	{
		strDestination[intIndex] = strSource[intSourceIndex];
		intIndex += 1;
		intSourceIndex += 1;
	}

	// end with null terminator
	strDestination[intIndex] = 0;
}

// --------------------------------------------------------------------------------
// Name: GetStringLength
// Abstract: Returns the length of the provided string
// --------------------------------------------------------------------------------
int GetStringLength(char strSource[])
{
	int intLength = 0;

	while (strSource[intLength] != 0)
	{
		intLength += 1;
	}

	return intLength;
}

// --------------------------------------------------------------------------------
// Name: SliceString
// Abstract: Copies a substring from the source string to the destination
// --------------------------------------------------------------------------------
void SliceString(char strDestination[], char strSource[], int intStartIndex, int intLength)
{
	int intSourceIndex;
	int intDestinationIndex = 0;
	int intEndIndex = intStartIndex + intLength;

	for (intSourceIndex = intStartIndex; intSourceIndex < intEndIndex; intSourceIndex += 1)
	{
		strDestination[intDestinationIndex] = strSource[intSourceIndex];
		intDestinationIndex += 1;
	}

	// end with null terminator
	strDestination[intLength] = 0;
}