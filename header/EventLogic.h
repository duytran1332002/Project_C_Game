#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "UserLogin.h"
FILE *fptr;

char *holidayList[] = {
	"Lunar New Year",
	"Hung King Fest",
	"Mid-year Fest",
	"Moon Fest",
	"Kitchen Guar",
	"New Year",
	"Valentine's Day",
	"Women's Day",
	"Liberation Day",
	"Workers' Day",
	"Children's Day",
	"National Day",
	"VNese Women",
	"Halloween",
	"Men's Day",
	"Teachers's Day",
	"Chirstmast Day",
};

char* solarHolidayHandler(int day, int month)
{
	switch (month)
	{
		case 1:
			if (day == 1) return holidayList[5];
			else return "";
		case 2:
			if (day == 14) return holidayList[6];
			else return "";
		case 3:
			if (day == 8) return holidayList[7];
			else return "";
		case 4:
			if (day == 30) return holidayList[8];
			else return "";
		case 5:
			if (day == 1) return holidayList[9];
			else return "";
		case 6:
			if (day == 1) return holidayList[10];
			else return "";
		case 9:
			if (day == 2) return holidayList[11];
			else return "";
		case 10:
			if (day == 20) return holidayList[12];
			else if (day == 31) return holidayList[13];
				else return "";
		case 11:
			if (day == 19) return holidayList[14];
			else if (day == 20) return holidayList[15];
				else return "";
		case 12:
			if (day == 24) return holidayList[16];
			else return "";
		default:
			return "";
	}
}

char* lunarHolidayHandler(int day, int month)
{
	switch (month)
	{
		case 1:
			if (day == 1) return holidayList[0];
			else return "";
		case 3:
			if (day == 10) return holidayList[1];
			else return "";
		case 5:
			if (day == 5) return holidayList[2];
			else return "";
		case 8:
			if (day == 15) return holidayList[3];
			else return "";
		case 12:
			if (day == 23) return holidayList[4];
			else return "";
		default:
			return "";
	}
}


void setEvent(int day, int month, int year, const char* eventTitle, char* eventContent, int featureID, int userID)
{
	char fileName[30];
	strcpy(fileName, "data/userEvent/user");
	char IDstr[2];
	sprintf(IDstr, "%d", userID);
	strcat(fileName, IDstr);
	strcat(fileName, ".dat");
	fptr = fopen(fileName, "r");

	int numOfLines = 0, flag = 0;
	char fetchedInformation[100][10000];
	char tempInfo[10000];

	while (fgets(tempInfo, 10000, fptr) != NULL)
	{
	    if (strlen(tempInfo) < 5) continue;
		numOfLines++;
		char *tempInfo1;
		tempInfo1 = strtok(tempInfo, "\n");
		strcpy(fetchedInformation[numOfLines], tempInfo1);
		int i = 0, j = 0;
        char *tokens[100];
        char *token;

        token = strtok(tempInfo1, "`");
		while (token != NULL) {
            tokens[i] = token;
            i++;
            token = strtok(NULL, "`");
        }
		int numDate[3];
		char *dateTokens[3];

        char *dateToken = strtok(tokens[0], "/");
        while (dateToken != NULL) {
            dateTokens[j] = dateToken;
            numDate[j] = atoi(dateTokens[j]);
            j++;
            dateToken = strtok(NULL, "/");
        }
		if (day == numDate[0] && month == numDate[1] && year == numDate[2])
		{
			flag = 1;
			char buf[10];
			itoa(featureID, buf, 10);
			strcat(fetchedInformation[numOfLines], "`");
			strcat(fetchedInformation[numOfLines], buf);
			strcat(fetchedInformation[numOfLines], "`");
			strcat(fetchedInformation[numOfLines], eventTitle);
			strcat(fetchedInformation[numOfLines], "`");
			strcat(fetchedInformation[numOfLines], eventContent);
		}
	}
	fclose(fptr);
	if (flag == 1) {
		fptr = fopen(fileName, "w");
		for (int i = 1; i <= numOfLines; i++) {
			fprintf(fptr, "%s\n", fetchedInformation[i]);
		}
		fclose(fptr);
	} else {
		char inputContent[1000];
		char buf[10];
		itoa(day, buf, 10);
		strcpy(inputContent, buf);
		strcat(inputContent, "/");
		itoa(month, buf, 10);
		strcat(inputContent, buf);
		strcat(inputContent, "/");
		itoa(year, buf, 10);
		strcat(inputContent, buf);
		strcat(inputContent, "`");
		itoa(featureID, buf, 10);
		strcat(inputContent, buf);
		strcat(inputContent, "`");
		strcat(inputContent, eventTitle);
		strcat(inputContent, "`");
		strcat(inputContent, eventContent);
		fptr = fopen(fileName, "a");
		fprintf(fptr, "%s\n", inputContent);
		fclose(fptr);
	}
	return;
}

char* getTaskbyDate(int userID, int day, int month, int year)
{
    static char* strTask;
	char fileName[30];
	strcpy(fileName, "data/userEvent/user");
	char IDstr[2];
	sprintf(IDstr, "%d", userID);
	strcat(fileName, IDstr);
	strcat(fileName, ".dat");
	fptr = fopen(fileName, "r");

	int numOfLines = 0, flag = 0;
	char fetchedInformation[100][10000];
	char tempInfo[10000];

	while (fgets(tempInfo, 10000, fptr) != NULL)
	{
		numOfLines++;
		char *tempInfo1;
		tempInfo1 = strtok(tempInfo, "");
		strcpy(fetchedInformation[numOfLines], tempInfo1);
		int i = 0, j = 0;
        char *tokens[100];
        char *token;

        token = strtok(tempInfo1, "`");
		while (token != NULL) {
            tokens[i] = token;
            i++;
            token = strtok(NULL, "`");
        }

		int numDate[3];
		char *dateTokens[3];

        char *dateToken = strtok(tokens[0], "/");
        while (dateToken != NULL) {
            dateTokens[j] = dateToken;
            numDate[j] = atoi(dateTokens[j]);
            j++;
            dateToken = strtok(NULL, "/");
        }
		if (day == numDate[0] && month == numDate[1] && year == numDate[2])
		{
			strTask = fetchedInformation[numOfLines];
			fclose(fptr);
			return strTask;
		}
	}
	fclose(fptr);
	return "NULL";
}

void clearEvent(int day, int month, int year, int eventID, int userID)
{
	char fileName[30];
	strcpy(fileName, "data/userEvent/user");
	char IDstr[2];
	sprintf(IDstr, "%d", userID);
	strcat(fileName, IDstr);
	strcat(fileName, ".dat");
	fptr = fopen(fileName, "r");

	int numOfLines = 0, flag = 0;
	char fetchedInformation[100][10000];
	char tempInfo[10000];
	while (fgets(tempInfo, 10000, fptr) != NULL)
	{
		numOfLines++;
		strcpy(fetchedInformation[numOfLines], tempInfo);
		int i = 0, j = 0;
        char *tokens[100];
        char *token;

        token = strtok(tempInfo, "`");
		while (token != NULL) {
            tokens[i] = token;
            i++;
            token = strtok(NULL, "`");
        }

		int numDate[3];
		char *dateTokens[3];
        char strDate[100];

		strcpy(strDate, tokens[0]);
        char *dateToken = strtok(strDate, "/");
        while (dateToken != NULL) {
            dateTokens[j] = dateToken;
            numDate[j] = atoi(dateTokens[j]);
            j++;
            dateToken = strtok(NULL, "/");
        }
		if (day == numDate[0] && month == numDate[1] && year == numDate[2])
		{
			flag = 1;
			if (i == 4) {
				strcpy(fetchedInformation[numOfLines], "");
			} else {
				strcpy(fetchedInformation[numOfLines], tokens[0]);

				for (int tokenIndex = 1; tokenIndex < i; tokenIndex++)
					if (tokenIndex != (eventID) && tokenIndex != (eventID + 1) && tokenIndex != (eventID + 2)) {
					strcat(fetchedInformation[numOfLines], "`");
					char* tempStr = tokens[tokenIndex];
					strcat(fetchedInformation[numOfLines], tempStr);
				}
				strcat(fetchedInformation[numOfLines], "\n");
			}
		}
	}

	fclose(fptr);
	if (flag == 1) {
		fptr = fopen(fileName, "w");
		for (int line = 1; line <= numOfLines; line++)
			fprintf(fptr, "%s", fetchedInformation[line]);
		fclose(fptr);
	} else {
        g_print("Error! Can't find event");
	}
	return;
}

void deleteUserByID(int userID) {
    char fileName[30];
	strcpy(fileName, "data/userEvent/user");
	char IDstr[2];
	sprintf(IDstr, "%d", userID);
	strcat(fileName, IDstr);
	strcat(fileName, ".dat");
    remove(fileName);
    int lastID = getLastUserID();
    for (int i = userID + 1; i <= lastID; i++) {
        char fileName1[30];
        strcpy(fileName1, "data/userEvent/user");
        char IDstr1[2];
        sprintf(IDstr1, "%d", i);
        strcat(fileName1, IDstr1);
        strcat(fileName1, ".dat");
        // kiem tra file co ton tai
        if ((fptr = fopen(fileName1, "r"))) {
            fclose(fptr);
            //dat ten moi
            char fileName2[30];
            strcpy(fileName2, "data/userEvent/user");
            char IDstr2[2];
            sprintf(IDstr2, "%d", i - 1);
            strcat(fileName2, IDstr2);
            strcat(fileName2, ".dat");
            rename(fileName1, fileName2);
        }
    }


}











