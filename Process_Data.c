// --------------------------------------------------------------------------------
// Name: David Simpson
// Class: SET 151-400
// Abstract: Final Project - Process Data
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Includes � built-in libraries of functions
// --------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

// --------------------------------------------------------------------------------
// Constants
// --------------------------------------------------------------------------------
double dblHOUSE_1_POVERTY = 12000.00;
double dblHOUSE_2_POVERTY = 18000.00;
double dblHOUSE_3_POVERTY = 25000.00;
double dblHOUSE_4_POVERTY = 30000.00;
double dblHOUSE_5PLUS_POVERTY = 40000.00;

// --------------------------------------------------------------------------------
// User Defined Types (UDT)
// --------------------------------------------------------------------------------
typedef struct
{
	char strDate[11];
	int intState;
	int intCountyOhio;
	int intCountyKentucky;
	int intRace;
	int intHouseHeadCount;
	double dblHouseIncome;
} udtCensusRecordType;

// --------------------------------------------------------------------------------
// Prototypes
// --------------------------------------------------------------------------------
int GetNumberOfRecords(FILE* pfilCensusData);
void CreateArray(udtCensusRecordType** ppaudtCensusRecords, int intArraySize);
void PopulateArrayFromFile(udtCensusRecordType** ppaudtCensusRecords, FILE* pfilCensusData, int intRecordCount);
void CopyFileLineAtIndex(char strDestination[], FILE* pfilCensusData, int intLine);
void CopyCSVatIndex(char strDestination[], char strSourceCSV[], int intIndexCSV);
char MainMenu();

void DisplayHousesByCounty(udtCensusRecordType** ppaudtCensusRecords, int intRecordCount);
void DisplayHousesByRace(udtCensusRecordType** ppaudtCensusRecords, int intRecordCount);
void DisplayAverageHouseIncome(udtCensusRecordType** ppaudtCensusRecords, int intRecordCount);
void DisplayAvgIncomeCountyState(udtCensusRecordType** ppaudtCensusRecords, int intRecordCount);
void DisplayAvgIncomeByRace(udtCensusRecordType** ppaudtCensusRecords, int intRecordCount);
void DisplayPercentBelowPoverty(udtCensusRecordType** ppaudtCensusRecords, int intRecordCount);
void DisplayPovertyByCounty(udtCensusRecordType** ppaudtCensusRecords, int intRecordCount);
void DisplayPovertyByRace(udtCensusRecordType** ppaudtCensusRecords, int intRecordCount);

void SliceString(char strDestination[], char strSource[], int intStartIndex, int intLength);
int GetStringLength(char strSource[]);
double GetPovertyThreshold(int intHeadCount);

// --------------------------------------------------------------------------------
// Name: main
// Abstract: This is where the program starts
// --------------------------------------------------------------------------------
void main()
{
	udtCensusRecordType* audtCensusRecords;
	FILE* pfilCensusData = fopen("res/census_data.txt", "r");
	
	int intRecordCount = GetNumberOfRecords(pfilCensusData);
	
	CreateArray(&audtCensusRecords, intRecordCount);
	PopulateArrayFromFile(&audtCensusRecords, pfilCensusData, intRecordCount);

	char chrChoice = MainMenu();

	switch (chrChoice)
	{
	case 'A':
		printf("Total Households surveyed: %d\n\n", intRecordCount);
		break;
	case 'B':
		DisplayHousesByCounty(&audtCensusRecords, intRecordCount);
		break;
	case 'C':
		DisplayHousesByRace(&audtCensusRecords, intRecordCount);
		break;
	case 'D':
		DisplayAverageHouseIncome(&audtCensusRecords, intRecordCount);
		break;
	case 'E':
		DisplayAvgIncomeCountyState(&audtCensusRecords, intRecordCount);
		break;
	case 'F':
		DisplayAvgIncomeByRace(&audtCensusRecords, intRecordCount);
		break;
	case 'G':
		DisplayPercentBelowPoverty(&audtCensusRecords, intRecordCount);
		break;
	case 'H':
		DisplayPovertyByCounty(&audtCensusRecords, intRecordCount);
		break;
	case 'I':
		DisplayPovertyByRace(&audtCensusRecords, intRecordCount);
	}

	free(audtCensusRecords);
	audtCensusRecords = 0;

	fclose(pfilCensusData);

	system("pause");
}

// --------------------------------------------------------------------------------
// Name: GetNumberOfRecords
// Abstract: Get number of census records, this will tell us how large to make the array
// --------------------------------------------------------------------------------
int GetNumberOfRecords(FILE* pfilCensusData)
{
	int intRecordCount = 0;

	char chrCurrent = 0;
	while (chrCurrent >= 0)
	{
		chrCurrent = fgetc(pfilCensusData);
		if (chrCurrent == '\n')
		{
			intRecordCount += 1;
		}
	}

	// reset file stream
	fseek(pfilCensusData, 0, SEEK_SET);

	return intRecordCount;
}

// --------------------------------------------------------------------------------
// Name: CreateArray
// Abstract: Create an array with size based on passed value
// --------------------------------------------------------------------------------
void CreateArray(udtCensusRecordType** ppaudtCensusRecords, int intArraySize)
{
	*ppaudtCensusRecords = (udtCensusRecordType*)malloc(sizeof(udtCensusRecordType) * intArraySize);
}

// --------------------------------------------------------------------------------
// Name: PopulateArrayFromFile
// Abstract: Populate array from passed file stream
// --------------------------------------------------------------------------------
void PopulateArrayFromFile(udtCensusRecordType** ppaudtCensusRecords, FILE* pfilCensusData, int intRecordCount)
{
	// maximum size a line can be is 36 + null terminator, per restrictions from Gather_Data.c
	char strCurrentLine[37] = "";
	char strState[2] = "";
	char strCountyOhio[2] = "";
	char strCountyKentucky[2] = "";
	char strRace[2] = "";
	char strHeadCount[3] = "";
	char strHouseIncome[16] = "";

	for (int intIndexLine = 0; intIndexLine < intRecordCount; intIndexLine += 1)
	{
		CopyFileLineAtIndex(strCurrentLine, pfilCensusData, intIndexLine);

		// load date
		CopyCSVatIndex((*ppaudtCensusRecords + intIndexLine)->strDate, strCurrentLine, 0);

		// load state
		CopyCSVatIndex(strState, strCurrentLine, 1);
		int intState = atoi(strState);
		(*ppaudtCensusRecords + intIndexLine)->intState = intState;

		// load ohio county
		CopyCSVatIndex(strCountyOhio, strCurrentLine, 2);
		int intCountyOhio = atoi(strCountyOhio);
		(*ppaudtCensusRecords + intIndexLine)->intCountyOhio = intCountyOhio;

		// load kentucky county
		CopyCSVatIndex(strCountyKentucky, strCurrentLine, 3);
		int intCountyKentucky = atoi(strCountyKentucky);
		(*ppaudtCensusRecords + intIndexLine)->intCountyKentucky = intCountyKentucky;

		// load race
		CopyCSVatIndex(strRace, strCurrentLine, 4);
		int intRace = atoi(strRace);
		(*ppaudtCensusRecords + intIndexLine)->intRace = intRace;

		// load head count
		CopyCSVatIndex(strHeadCount, strCurrentLine, 5);
		int intHeadCount = atoi(strHeadCount);
		(*ppaudtCensusRecords + intIndexLine)->intHouseHeadCount = intHeadCount;

		// load income
		CopyCSVatIndex(strHouseIncome, strCurrentLine, 6);
		double dblHouseIncome = atof(strHouseIncome);
		(*ppaudtCensusRecords + intIndexLine)->dblHouseIncome = dblHouseIncome;
	}

	// reset file stream
	fseek(pfilCensusData, 0, SEEK_SET);
}

// --------------------------------------------------------------------------------
// Name: GetFileLineAtIndex
// Abstract: Copies the specified line from a text file, first line is 0
// --------------------------------------------------------------------------------
void CopyFileLineAtIndex(char strDestination[], FILE* pfilCensusData, int intLine)
{
	int intRecordCount = 0;

	char chrCurrent = 0;
	// loop until we reach end of file
	while (chrCurrent >= 0)
	{
		chrCurrent = fgetc(pfilCensusData);
		if (chrCurrent == '\n')
		{
			intRecordCount += 1;
		}

		if (intRecordCount == intLine)
		{	
			// advance fgetc pointer past previous newline, but only if not the first line
			if (intRecordCount != 0)
			{
				chrCurrent = fgetc(pfilCensusData);
			}
			int intIndex = 0;
			while (chrCurrent != '\n')
			{
				strDestination[intIndex] = chrCurrent;
				chrCurrent = fgetc(pfilCensusData);
				intIndex += 1;
			}
			// don't forget the null terminator
			strDestination[intIndex] = 0;
			break;
		}
	}

	// reset file stream
	fseek(pfilCensusData, 0, SEEK_SET);
}

// --------------------------------------------------------------------------------
// Name: GetCSVatIndex
// Abstract: Gets the comma separated value at specified index
// --------------------------------------------------------------------------------
void CopyCSVatIndex(char strDestination[], char strSourceCSV[], int intIndexCSV)
{
	int intDelimiterCount = 0;
	int intSourceCSVLength = GetStringLength(strSourceCSV);
	int intStart = 0;
	int intStartFlag = 0;
	int intEnd = 0;

	// loop through the array, find the start and end for the intIndexCSV, then slice it out
	for (int intIndex = 0; intIndex <= intSourceCSVLength; intIndex += 1)
	{
		if (intDelimiterCount == intIndexCSV && !intStartFlag)
		{
			intStart = intIndex;
			intStartFlag = 1;
		}

		if (strSourceCSV[intIndex] == ',' || strSourceCSV[intIndex] == 0)
		{
			intDelimiterCount += 1;
			if (intDelimiterCount - 1 == intIndexCSV)
			{
				intEnd = intIndex;
				break;
			}
		}
	}
	int intLength = intEnd - intStart;

	SliceString(strDestination, strSourceCSV, intStart, intLength);
}

// --------------------------------------------------------------------------------
// Name: MainMenu
// Abstract: Shows the main menu options, returns the choice as letter A through I
// --------------------------------------------------------------------------------
char MainMenu()
{	
	char chrChoice = 0;

	printf(
		"Which data set would you like to display?\n"
		"A. Total Households Surveyed\n"
		"B. Total Households Surveyed per County\n"
		"C. Total Households Surveyed per Race\n"
		"D. Average Household Income\n"
		"E. Average Household Income by county and state\n"
		"F. Average Household Income by race\n"
		"G. Percentage below Poverty\n"
		"H. Percentage below Poverty by county and state\n"
		"I. Percentage below Poverty by race\n"
	);

	// get choice
	while (chrChoice < 'A' || chrChoice > 'I')
	{
		printf("\nPlease make your choice by choosing a letter from above: ");
		scanf(" %c", &chrChoice);

		// if choice is valid but lower case, make upper case
		if (chrChoice >= 'a' && chrChoice <= 'i')
		{
			chrChoice -= 32;
		}
	}

	return chrChoice;
}

// --------------------------------------------------------------------------------
// Name: DisplayHousesByCounty
// Abstract: Shows how many houses are in each county
// --------------------------------------------------------------------------------
void DisplayHousesByCounty(udtCensusRecordType** ppaudtCensusRecords, int intRecordCount)
{
	int intHamiltonCount = 0;
	int intButlerCount = 0;
	int intBooneCount = 0;
	int intKentonCount = 0;

	for (int intIndex = 0; intIndex < intRecordCount; intIndex += 1)
	{
		switch ((*ppaudtCensusRecords + intIndex)->intState)
		{
		case 1:
			switch ((*ppaudtCensusRecords + intIndex)->intCountyOhio)
			{
			case 1:
				intHamiltonCount += 1;
				break;
			case 2:
				intButlerCount += 1;
			}
			break;
		case 2:
			switch ((*ppaudtCensusRecords + intIndex)->intCountyKentucky)
			{
			case 1:
				intBooneCount += 1;
				break;
			case 2:
				intKentonCount += 1;
			}
		}
	}

	printf("==Ohio households==\n");
	printf("Hamilton County: %d\n", intHamiltonCount);
	printf("Butler County: %d\n\n", intButlerCount);
	printf("==Kentucky households==\n");
	printf("Boone County: %d\n", intBooneCount);
	printf("Kenton County: %d\n\n", intKentonCount);
}

// --------------------------------------------------------------------------------
// Name: DisplayHousesByRace
// Abstract: Shows how many houses per race
// --------------------------------------------------------------------------------
void DisplayHousesByRace(udtCensusRecordType** ppaudtCensusRecords, int intRecordCount)
{
	int intCaucasian = 0;
	int intAfricanAmerican = 0;
	int intHispanic = 0;
	int intAsian = 0;
	int intOther = 0;

	for (int intIndex = 0; intIndex < intRecordCount; intIndex += 1)
	{
		switch ((*ppaudtCensusRecords + intIndex)->intRace)
		{
		case 1:
			intCaucasian += 1;
			break;
		case 2:
			intAfricanAmerican += 1;
			break;
		case 3:
			intHispanic += 1;
			break;
		case 4:
			intAsian += 1;
			break;
		case 5:
			intOther += 1;
		}
	}

	printf("==Houses by Race==\n");
	printf("Caucasian: %d\n", intCaucasian);
	printf("African American: %d\n", intAfricanAmerican);
	printf("Hispanic: %d\n", intHispanic);
	printf("Asian: %d\n", intAsian);
	printf("Other: %d\n\n", intOther);
}

// --------------------------------------------------------------------------------
// Name: DisplayAverageHouseIncome
// Abstract: Shows average household income
// --------------------------------------------------------------------------------
void DisplayAverageHouseIncome(udtCensusRecordType** ppaudtCensusRecords, int intRecordCount)
{
	double dblTotalIncome = 0.0;

	for (int intIndex = 0; intIndex < intRecordCount; intIndex += 1)
	{
		dblTotalIncome += (*ppaudtCensusRecords + intIndex)->dblHouseIncome;
	}

	double dblAverageIncome = dblTotalIncome / intRecordCount;

	printf("==Average Income==\n");
	printf("$%.2f\n\n", dblAverageIncome);
}

// --------------------------------------------------------------------------------
// Name: DisplayAvgIncomeCountyState
// Abstract: Shows average household income by county and state
// --------------------------------------------------------------------------------
void DisplayAvgIncomeCountyState(udtCensusRecordType** ppaudtCensusRecords, int intRecordCount)
{
	int intHamiltonCount = 0;
	int intButlerCount = 0;
	int intBooneCount = 0;
	int intKentonCount = 0;

	double dblHamiltonIncome = 0.0;
	double dblButlerIncome = 0.0;
	double dblBooneIncome = 0.0;
	double dblKentonIncome = 0.0;

	for (int intIndex = 0; intIndex < intRecordCount; intIndex += 1)
	{
		switch ((*ppaudtCensusRecords + intIndex)->intState)
		{
		case 1:
			switch ((*ppaudtCensusRecords + intIndex)->intCountyOhio)
			{
			case 1:
				intHamiltonCount += 1;
				dblHamiltonIncome += (*ppaudtCensusRecords + intIndex)->dblHouseIncome;
				break;
			case 2:
				intButlerCount += 1;
				dblButlerIncome += (*ppaudtCensusRecords + intIndex)->dblHouseIncome;
			}
			break;
		case 2:
			switch ((*ppaudtCensusRecords + intIndex)->intCountyKentucky)
			{
			case 1:
				intBooneCount += 1;
				dblBooneIncome += (*ppaudtCensusRecords + intIndex)->dblHouseIncome;
				break;
			case 2:
				intKentonCount += 1;
				dblKentonIncome += (*ppaudtCensusRecords + intIndex)->dblHouseIncome;
			}
		}
	}

	double dblHamiltonAverage = 0.0;
	double dblButlerAverage = 0.0;
	double dblBooneAverage = 0.0;
	double dblKentonAverage = 0.0;

	// checks to avoid dividing by 0
	if (intHamiltonCount > 0)
	{
		dblHamiltonAverage = dblHamiltonIncome / intHamiltonCount;
	}
	if (intButlerCount > 0)
	{
		dblButlerAverage = dblButlerIncome / intButlerCount;
	}
	if (intBooneCount > 0)
	{
		dblBooneAverage = dblBooneIncome / intBooneCount;
	}
	if (intKentonCount > 0)
	{
		dblKentonAverage = dblKentonIncome / intKentonCount;
	}

	printf("==Average Ohio Income by County==\n");
	printf("Hamilton: $%.2f\n", dblHamiltonAverage);
	printf("Butler: $%.2f\n\n", dblButlerAverage);
	printf("==Average Kentucky Income by County==\n");
	printf("Boone: $%.2f\n", dblBooneAverage);
	printf("Kenton: $%.2f\n\n", dblKentonAverage);
}

// --------------------------------------------------------------------------------
// Name: DisplayAvgIncomeByRace
// Abstract: Shows average income per race
// --------------------------------------------------------------------------------
void DisplayAvgIncomeByRace(udtCensusRecordType** ppaudtCensusRecords, int intRecordCount)
{
	int intCaucasian = 0;
	int intAfricanAmerican = 0;
	int intHispanic = 0;
	int intAsian = 0;
	int intOther = 0;

	double dblCaucasionIncome = 0.0;
	double dblAfricanAmericanIncome = 0.0;
	double dblHispanicIncome = 0.0;
	double dblAsianIncome = 0.0;
	double dblOtherIncome = 0.0;

	for (int intIndex = 0; intIndex < intRecordCount; intIndex += 1)
	{
		switch ((*ppaudtCensusRecords + intIndex)->intRace)
		{
		case 1:
			intCaucasian += 1;
			dblCaucasionIncome += (*ppaudtCensusRecords + intIndex)->dblHouseIncome;
			break;
		case 2:
			intAfricanAmerican += 1;
			dblAfricanAmericanIncome += (*ppaudtCensusRecords + intIndex)->dblHouseIncome;
			break;
		case 3:
			intHispanic += 1;
			dblHispanicIncome += (*ppaudtCensusRecords + intIndex)->dblHouseIncome;
			break;
		case 4:
			intAsian += 1;
			dblAsianIncome += (*ppaudtCensusRecords + intIndex)->dblHouseIncome;
			break;
		case 5:
			intOther += 1;
			dblOtherIncome += (*ppaudtCensusRecords + intIndex)->dblHouseIncome;
		}
	}

	double dblCaucasianAverage = 0.0;
	double dblAfricanAmericanAverage = 0.0;
	double dblHispanicAverage = 0.0;
	double dblAsianAverage = 0.0;
	double dblOtherAverage = 0.0;

	// checks to avoid dividing by 0
	if (intCaucasian > 0)
	{
		dblCaucasianAverage = dblCaucasionIncome / intCaucasian;
	}
	if (intAfricanAmerican > 0)
	{
		dblAfricanAmericanAverage = dblAfricanAmericanIncome / intAfricanAmerican;
	}
	if (intHispanic > 0)
	{
		dblHispanicAverage = dblHispanicIncome / intHispanic;
	}
	if (intAsian > 0)
	{
		dblAsianAverage = dblAsianIncome / intAsian;
	}
	if (intOther > 0)
	{
		dblOtherAverage = dblOtherIncome / intOther;
	}

	printf("==Average Income by Race==\n");
	printf("Caucasian: $%.2f\n", dblCaucasianAverage);
	printf("African American: $%.2f\n", dblAfricanAmericanAverage);
	printf("Hispanic: $%.2f\n", dblHispanicAverage);
	printf("Asian: $%.2f\n", dblAsianAverage);
	printf("Other: $%.2f\n\n", dblOtherAverage);
}

// --------------------------------------------------------------------------------
// Name: DisplayPercentBelowPoverty
// Abstract: Shows the percentage of households below the poverty line
// --------------------------------------------------------------------------------
void DisplayPercentBelowPoverty(udtCensusRecordType** ppaudtCensusRecords, int intRecordCount)
{
	double dblTotalInPoverty = 0.0;

	for (int intIndex = 0; intIndex < intRecordCount; intIndex += 1)
	{
		if (GetPovertyThreshold((*ppaudtCensusRecords + intIndex)->intHouseHeadCount) > (*ppaudtCensusRecords + intIndex)->dblHouseIncome)
		{
			dblTotalInPoverty += 1;
		}
	}

	double dblPercent = 100 * (dblTotalInPoverty / intRecordCount);

	printf("==Percent in Poverty==\n");
	printf("%.2f%%\n\n", dblPercent);
}

// --------------------------------------------------------------------------------
// Name: DisplayPovertyByCounty
// Abstract: Shows the percentage of households below the poverty line per county
// --------------------------------------------------------------------------------
void DisplayPovertyByCounty(udtCensusRecordType** ppaudtCensusRecords, int intRecordCount)
{
	int intHamiltonCount = 0;
	int intButlerCount = 0;
	int intBooneCount = 0;
	int intKentonCount = 0;
	
	double dblHamiltonPoverty = 0.0;
	double dblButlerPoverty = 0.0;
	double dblBoonePoverty = 0.0;
	double dblKentonPoverty = 0.0;

	for (int intIndex = 0; intIndex < intRecordCount; intIndex += 1)
	{
		switch ((*ppaudtCensusRecords + intIndex)->intState)
		{
		case 1:
			switch ((*ppaudtCensusRecords + intIndex)->intCountyOhio)
			{
			case 1:
				intHamiltonCount += 1;
				if (GetPovertyThreshold((*ppaudtCensusRecords + intIndex)->intHouseHeadCount) > (*ppaudtCensusRecords + intIndex)->dblHouseIncome)
				{
					dblHamiltonPoverty += 1;
				}
				break;
			case 2:
				intButlerCount += 1;
				if (GetPovertyThreshold((*ppaudtCensusRecords + intIndex)->intHouseHeadCount) > (*ppaudtCensusRecords + intIndex)->dblHouseIncome)
				{
					dblButlerPoverty += 1;
				}
			}
			break;
		case 2:
			switch ((*ppaudtCensusRecords + intIndex)->intCountyKentucky)
			{
			case 1:
				intBooneCount += 1;
				if (GetPovertyThreshold((*ppaudtCensusRecords + intIndex)->intHouseHeadCount) > (*ppaudtCensusRecords + intIndex)->dblHouseIncome)
				{
					dblBoonePoverty += 1;
				}
				break;
			case 2:
				intKentonCount += 1;
				if (GetPovertyThreshold((*ppaudtCensusRecords + intIndex)->intHouseHeadCount) > (*ppaudtCensusRecords + intIndex)->dblHouseIncome)
				{
					dblKentonPoverty += 1;
				}
			}
		}
	}

	double dblHamiltonPercent = 0.0;
	double dblButlerPercent = 0.0;
	double dblBoonePercent = 0.0;
	double dblKentonPercent = 0.0;

	// divide by zero checks
	if (intHamiltonCount > 0)
	{
		dblHamiltonPercent = 100 * (dblHamiltonPoverty / intHamiltonCount);
	}
	if (intButlerCount > 0)
	{
		dblButlerPercent = 100 * (dblButlerPoverty / intButlerCount);
	}
	if (intBooneCount > 0)
	{
		dblBoonePercent = 100 * (dblBoonePoverty / intBooneCount);
	}
	if (intKentonCount > 0)
	{
		dblKentonPercent = 100 * (dblKentonPoverty / intKentonCount);
	}


	printf("==Ohio Poverty Percentages==\n");
	printf("Hamilton County: %.2f%%\n", dblHamiltonPercent);
	printf("Butler County: %.2f%%\n\n", dblButlerPercent);
	printf("==Kentucky Poverty Percentages==\n");
	printf("Boone County: %.2f%%\n", dblBoonePercent);
	printf("Kenton County: %.2f%%\n\n", dblKentonPercent);
}

// --------------------------------------------------------------------------------
// Name: DisplayPovertyByRace
// Abstract: Shows the percentage of households below the poverty line per race
// --------------------------------------------------------------------------------
void DisplayPovertyByRace(udtCensusRecordType** ppaudtCensusRecords, int intRecordCount)
{
	int intCaucasian = 0;
	int intAfricanAmerican = 0;
	int intHispanic = 0;
	int intAsian = 0;
	int intOther = 0;

	double dblCaucasionPoverty = 0.0;
	double dblAfricanAmericanPoverty = 0.0;
	double dblHispanicPoverty = 0.0;
	double dblAsianPoverty = 0.0;
	double dblOtherPoverty = 0.0;

	for (int intIndex = 0; intIndex < intRecordCount; intIndex += 1)
	{
		switch ((*ppaudtCensusRecords + intIndex)->intRace)
		{
		case 1:
			intCaucasian += 1;
			if (GetPovertyThreshold((*ppaudtCensusRecords + intIndex)->intHouseHeadCount) > (*ppaudtCensusRecords + intIndex)->dblHouseIncome)
			{
				dblCaucasionPoverty += 1;
			}
			break;
		case 2:
			intAfricanAmerican += 1;
			if (GetPovertyThreshold((*ppaudtCensusRecords + intIndex)->intHouseHeadCount) > (*ppaudtCensusRecords + intIndex)->dblHouseIncome)
			{
				dblAfricanAmericanPoverty += 1;
			}
			break;
		case 3:
			intHispanic += 1;
			if (GetPovertyThreshold((*ppaudtCensusRecords + intIndex)->intHouseHeadCount) > (*ppaudtCensusRecords + intIndex)->dblHouseIncome)
			{
				dblHispanicPoverty += 1;
			}
			break;
		case 4:
			intAsian += 1;
			if (GetPovertyThreshold((*ppaudtCensusRecords + intIndex)->intHouseHeadCount) > (*ppaudtCensusRecords + intIndex)->dblHouseIncome)
			{
				dblAsianPoverty += 1;
			}
			break;
		case 5:
			intOther += 1;
			if (GetPovertyThreshold((*ppaudtCensusRecords + intIndex)->intHouseHeadCount) > (*ppaudtCensusRecords + intIndex)->dblHouseIncome)
			{
				dblOtherPoverty += 1;
			}
		}
	}

	double dblCaucasionPercent = 0.0;
	double dblAfricanAmericanPercent = 0.0;
	double dblHispanicPercent = 0.0;
	double dblAsianPercent = 0.0;
	double dblOtherPercent = 0.0;

	// divide by zero checks
	if (intCaucasian > 0)
	{
		dblCaucasionPercent = 100 * (dblCaucasionPoverty / intCaucasian);
	}
	if (intAfricanAmerican > 0)
	{
		dblAfricanAmericanPercent = 100 * (dblAfricanAmericanPoverty / intAfricanAmerican);
	}
	if (intHispanic > 0)
	{
		dblHispanicPercent = 100 * (dblHispanicPoverty / intHispanic);
	}
	if (intAsian > 0)
	{
		dblAsianPercent = 100 * (dblAsianPoverty / intAsian);
	}
	if (intOther > 0)
	{
		dblOtherPercent = 100 * (dblOtherPoverty / intOther);
	}

	printf("==Poverty by Race==\n");
	printf("Caucasian: %.2f%%\n", dblCaucasionPercent);
	printf("African American: %.2f%%\n", dblAfricanAmericanPercent);
	printf("Hispanic: %.2f%%\n", dblHispanicPercent);
	printf("Asian: %.2f%%\n", dblAsianPercent);
	printf("Other: %.2f%%\n\n", dblOtherPercent);

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
// Name: GetPovertyThreshold
// Abstract: Returns the poverty threshold given a household's headcount
// --------------------------------------------------------------------------------
double GetPovertyThreshold(int intHeadCount)
{
	double dblThreshold = 0.0;

	switch (intHeadCount)
	{
	case 1:
		dblThreshold = dblHOUSE_1_POVERTY;
		break;
	case 2:
		dblThreshold = dblHOUSE_2_POVERTY;
		break;
	case 3:
		dblThreshold = dblHOUSE_3_POVERTY;
		break;
	case 4:
		dblThreshold = dblHOUSE_4_POVERTY;
		break;
	default:
		dblThreshold = dblHOUSE_5PLUS_POVERTY;
	}

	return dblThreshold;
}