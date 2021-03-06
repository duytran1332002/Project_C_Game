#ifndef USER_LOGIN_H
#define USER_LOGIN_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "LunarCalendar.h"
#include "../model/UserModel.h"

/**
 * Prototypes
 */
// void parseDateOfBirth(char *dateString, Date *dateOfBirth);
int indexOf(char character, char *string);
void getUserByID(int id, User *fetchedUser);
void getUserByUsername(char *username, User *fetchedUser);
void getUserByName(char *name, User *fetchedUser);
int getLastUserID();
bool isUserIDExisted(int id);
bool isUserUsernameExisted(char *username);
int insertUser(User user);
//bool isValidDateOfBirth(Date dateOfBirth);
bool isValidName(char *name);
bool isValidUsername(char *username);
bool isValidPassword(char *password);
int userLogin(char *username, char *password);
int registerNewAccount(User user);
void modifyName(char *name);

FILE *userDataFile;
const char USER_FILE[] = "data/users_data.dat";
// int MAX_DAYS_OF_MONTH[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/**
 * parseDateOfBirth:
 * Parse the date of birth from dateString with format dd/mm/yyyy.
 * Then store data to dateOfBirth.
 */
// void parseDateOfBirth(char *dateString, Date *dateOfBirth) {
//     int j = 0;
//     char *tempPointer;
//     char *dateOfBirthTokens[3];
//     //Parse the first token
//     char *dateOfBirthToken = strtok(dateString, "/");
//     //Parse the next tokens
//     while (dateOfBirthToken != NULL) {
//         dateOfBirthTokens[j] = dateOfBirthToken;
//         j++;
//         dateOfBirthToken = strtok(NULL, "/");
//     }

//     dateOfBirth->day = (int) strtol(dateOfBirthTokens[0], &tempPointer, 10);
//     dateOfBirth->month = (int) strtol(dateOfBirthTokens[1], &tempPointer, 10);
//     dateOfBirth->year = (int) strtol(dateOfBirthTokens[2], &tempPointer, 10);
// }

/**
 * indexOf:
 * Return index of a character in the given string.
 */
int indexOf(char character, char *string) {
    char *charPosition = strchr(string, character);

    return charPosition == NULL ? -1 : (int) (charPosition - string);
}

/**
 * getUserByID:
 * Search for User with the given id. Then get information and store into fetchedUser.
 * If not found, set User's information to default value (0 and NULL).
 */
void getUserByID(int id, User *fetchedUser) {
    userDataFile = fopen(USER_FILE, "r");

    fetchedUser->id = 0;
    fetchedUser->name = NULL;
    fetchedUser->username = NULL;
    fetchedUser->password = NULL;
    // fetchedUser->dateOfBirth.day = 0;
    // fetchedUser->dateOfBirth.month = 0;
    // fetchedUser->dateOfBirth.year = 0;

    if (userDataFile != NULL) {
        char fetchedInformation[1000];

        /*
         * Loop to get every line in file
         */
        while (fgets(fetchedInformation, 1000, userDataFile) != NULL) {
            int endLineIndex = indexOf('\n', fetchedInformation);
            fetchedInformation[endLineIndex] = '\0';
            int i = 0, j = 0;
            char *tempPointer;

            /*
             * Parse main tokens (id-name-username-password)
             */
            char *tokens[4];
            char *token;
            //Parse the first token
            token = strtok(fetchedInformation, "-");
            //Parse the next tokens
            while (token != NULL) {
                tokens[i] = token;
                i++;
                token = strtok(NULL, "-");
            }

            //Check whether file has the given id or not
            if ((int) strtol(tokens[0], &tempPointer, 10) == id) {
                /*
                 * Parse date of birth tokens (day/month/year)
                 */
                // char *dateOfBirthTokens[3];
                // //Parse the first token
                // char *dateOfBirthToken = strtok(tokens[4], "/");
                // //Parse the next tokens
                // while (dateOfBirthToken != NULL) {
                //     dateOfBirthTokens[j] = dateOfBirthToken;
                //     j++;
                //     dateOfBirthToken = strtok(NULL, "/");
                // }

                //Store user's info into struct
                fetchedUser->id = (int) strtol(tokens[0], &tempPointer, 10);
                fetchedUser->name = tokens[1];
                fetchedUser->username = tokens[2];
                fetchedUser->password = tokens[3];

                // //Store date of birth into struct
                // fetchedUser->dateOfBirth.day = (int) strtol(dateOfBirthTokens[0], &tempPointer, 10);
                // fetchedUser->dateOfBirth.month = (int) strtol(dateOfBirthTokens[1], &tempPointer, 10);
                // fetchedUser->dateOfBirth.year = (int) strtol(dateOfBirthTokens[2], &tempPointer, 10);

                break;
            }
        }

        fclose(userDataFile);
    } else {
        printf("Error!! File is not existed.\n");
    }
}

/**
 * getUserByID:
 * Search for User with the given name. Then gets information and stores into fetchedUser.
 * If not found, set User's information to default value (0 and NULL).
 */
void getUserByName(char *givenName, User *fetchedUser) {
    userDataFile = fopen(USER_FILE, "r");

    fetchedUser->id = 0;
    fetchedUser->name = NULL;
    fetchedUser->username = NULL;
    fetchedUser->password = NULL;
    // fetchedUser->dateOfBirth.day = 0;
    // fetchedUser->dateOfBirth.month = 0;
    // fetchedUser->dateOfBirth.year = 0;

    if (userDataFile != NULL) {
        char fetchedInformation[1000];

        /*
         * Loop to get every line in file
         */
        while (fgets(fetchedInformation, 1000, userDataFile) != NULL) {
            int endLineIndex = indexOf('\n', fetchedInformation);
            fetchedInformation[endLineIndex] = '\0';
            int i = 0, j = 0;
            char *tempPointer;

            /*
             * Parse main tokens (id-name-username-password)
             */
            char *tokens[4];
            char *token;
            //Parse the first token
            token = strtok(fetchedInformation, "-");
            //Parse the next tokens
            while (token != NULL) {
                tokens[i] = token;
                i++;
                token = strtok(NULL, "-");
            }

            //Check whether file has the given username or not
            if (strcmp(tokens[1], givenName) == 0) {
                /*
                 * Parse date of birth tokens (day/month/year)
                 */
                // char *dateOfBirthTokens[3];
                // //Parse the first token
                // char *dateOfBirthToken = strtok(tokens[4], "/");
                // //Parse the next tokens
                // while (dateOfBirthToken != NULL) {
                //     dateOfBirthTokens[j] = dateOfBirthToken;
                //     j++;
                //     dateOfBirthToken = strtok(NULL, "/");
                // }

                //Store user's info into struct
                fetchedUser->id = (int) strtol(tokens[0], &tempPointer, 10);
                fetchedUser->name = tokens[1];
                fetchedUser->username = tokens[2];
                fetchedUser->password = tokens[3];

                // //Store date of birth into struct
                // fetchedUser->dateOfBirth.day = (int) strtol(dateOfBirthTokens[0], &tempPointer, 10);
                // fetchedUser->dateOfBirth.month = (int) strtol(dateOfBirthTokens[1], &tempPointer, 10);
                // fetchedUser->dateOfBirth.year = (int) strtol(dateOfBirthTokens[2], &tempPointer, 10);

                break;
            }
        }

        fclose(userDataFile);
    } else {
        printf("Error!! File is not existed.\n");
    }
}

/**
 * getUserByID:
 * Search for User with the given username. Then gets information and stores into fetchedUser.
 * If not found, set User's information to default value (0 and NULL).
 */
void getUserByUsername(char *givenUsername, User *fetchedUser) {
    userDataFile = fopen(USER_FILE, "r");

    fetchedUser->id = 0;
    fetchedUser->name = NULL;
    fetchedUser->username = NULL;
    fetchedUser->password = NULL;
    // fetchedUser->dateOfBirth.day = 0;
    // fetchedUser->dateOfBirth.month = 0;
    // fetchedUser->dateOfBirth.year = 0;

    if (userDataFile != NULL) {
        char fetchedInformation[1000];

        /*
         * Loop to get every line in file
         */
        while (fgets(fetchedInformation, 1000, userDataFile) != NULL) {
            int endLineIndex = indexOf('\n', fetchedInformation);
            fetchedInformation[endLineIndex] = '\0';
            int i = 0, j = 0;
            char *tempPointer;

            /*
             * Parse main tokens (id-name-username-password)
             */
            char *tokens[4];
            char *token;
            //Parse the first token
            token = strtok(fetchedInformation, "-");
            //Parse the next tokens
            while (token != NULL) {
                tokens[i] = token;
                i++;
                token = strtok(NULL, "-");
            }

            //Check whether file has the given username or not
            if (strcmp(tokens[2], givenUsername) == 0) {
                /*
                 * Parse date of birth tokens (day/month/year)
                 */
                // char *dateOfBirthTokens[3];
                // //Parse the first token
                // char *dateOfBirthToken = strtok(tokens[4], "/");
                // //Parse the next tokens
                // while (dateOfBirthToken != NULL) {
                //     dateOfBirthTokens[j] = dateOfBirthToken;
                //     j++;
                //     dateOfBirthToken = strtok(NULL, "/");
                // }

                //Store user's info into struct
                fetchedUser->id = (int) strtol(tokens[0], &tempPointer, 10);
                fetchedUser->name = tokens[1];
                fetchedUser->username = tokens[2];
                fetchedUser->password = tokens[3];

                // //Store date of birth into struct
                // fetchedUser->dateOfBirth.day = (int) strtol(dateOfBirthTokens[0], &tempPointer, 10);
                // fetchedUser->dateOfBirth.month = (int) strtol(dateOfBirthTokens[1], &tempPointer, 10);
                // fetchedUser->dateOfBirth.year = (int) strtol(dateOfBirthTokens[2], &tempPointer, 10);

                break;
            }
        }

        fclose(userDataFile);
    } else {
        printf("Error!! File is not existed.\n");
    }
}

/**
 * getLastUserID:
 * Return the last User's ID in the file. If failed, return -1.
 */
int getLastUserID() {
    userDataFile = fopen(USER_FILE, "r");
    int lastID = 0;

    if (userDataFile != NULL) {
        char fetchedInformation[1000];

        /*
         * Loop to get every line in file
         */
        while (fgets(fetchedInformation, 1000, userDataFile) != NULL) {
            int endLineIndex = indexOf('\n', fetchedInformation);
            fetchedInformation[endLineIndex] = '\0';
            int i = 0;
            char *tempPointer;

            /*
             * Parse the id
             */
            char *token;
            token = strtok(fetchedInformation, "-"); 
            lastID = (int) strtol(token, &tempPointer, 10);   
        }

        fclose(userDataFile);
        return lastID;
    } else {
        printf("Error!! File is not existed.\n");
        return -1;
    }
}

/**
 * isUserIDExisted:
 * Check whether a User with the given ID exists.
 */
bool isUserIDExisted(int id) {
    if (id < 1)
        return false;

    userDataFile = fopen(USER_FILE, "r");

    if (userDataFile != NULL) {
        char fetchedInformation[1000];

        /*
         * Loop to get every line in file
         */
        while (fgets(fetchedInformation, 1000, userDataFile) != NULL) {
            int endLineIndex = indexOf('\n', fetchedInformation);
            fetchedInformation[endLineIndex] = '\0';
            char *tempPointer;

            /*
             * Parse the first token containing the id
             */
            char *token;
            token = strtok(fetchedInformation, "-");

            //Compare to the parameter
            if ((int) strtol(token, &tempPointer, 10) == id) {
                fclose(userDataFile);
                return true;
            }
        }

        fclose(userDataFile);
        return false;
    } else {
        printf("Error!! File is not existed.\n");
    }
}

/**
 * isUserUsernameExisted:
 * Check whether a User with the given name exists.
 */
bool isUserUsernameExisted(char *username) {
    userDataFile = fopen(USER_FILE, "r");

    if (userDataFile != NULL) {
        char fetchedInformation[1000];

        /*
         * Loop to get every line in file
         */
        while (fgets(fetchedInformation, 1000, userDataFile) != NULL) {
            int endLineIndex = indexOf('\n', fetchedInformation);
            fetchedInformation[endLineIndex] = '\0';
            char *tempPointer;
            int i = 0;

            /*
             * Parse the first three tokens (id-name-username)
             */
            char *tokens[3];
            char *token;
            //Parse the first token
            token = strtok(fetchedInformation, "-");
            //Parse the rest
            while (token != NULL && i < 3) {
                tokens[i] = token;
                i++;
                token = strtok(NULL, "-");
            }

            //Compare to the parameter
            if (strcmp(tokens[2], username) == 0) {
                fclose(userDataFile);
                return true;
            }
        }

        fclose(userDataFile);
        return false;
    } else {
        printf("Error!! File is not existed.\n");
    }
}

/**
 * insertUser:
 * Append a User to the end of file. 
 * If insert successfully, return 1.
 * If there is already a user with the same username, return 0. 
 * If the parameter user is NULL, return -1.
 */
int insertUser(User user) {
    if (user.name == NULL || user.username == NULL || user.password == NULL) {
        return -1;
    }

    if (isUserUsernameExisted(user.username)) {
        return 0;
    } else {
        user.id = getLastUserID() + 1;

        userDataFile = fopen(USER_FILE, "a");
        
        modifyName(user.name);

        fprintf(userDataFile, "%d-%s-%s-%s\n", user.id, user.name, 
                user.username, user.password);

        fclose(userDataFile);        
        return 1;
    }
}

/**
 * isValidDateOfBirth:
 * Check whether the given date of birth is valid.
 */
// bool isValidDateOfBirth(Date dateOfBirth) {
// 	if (dateOfBirth.year < 1 || dateOfBirth.month < 1 || dateOfBirth.month > 12 || dateOfBirth.day < 1) 
// 		return false;

// 	if (dateOfBirth.month == 2 && isSolarLeapYear(dateOfBirth.year)) {
// 		if (dateOfBirth.day > 29)
// 		    return false;
// 	} else {
// 		if (dateOfBirth.day > MAX_DAYS_OF_MONTH[dateOfBirth.month - 1])
// 		    return false;
// 	}
	
//     return true;
// }

/**
 * isValidName:
 * Check whether the given name is valid.
 */
bool isValidName(char *name) {
	int leng = strlen(name);

	//Check range
	if (leng > 40 || leng < 1)
		return false;

	//Check special character and number
	for (int i = 0; i < leng; i++) {
		char c = name[i];
		if ( ( c < 'A' || ( c < 'Z' && c > 'a' ) || c > 'z' ) && c != ' ' ) 
			return false;
	}

	//Remove space
	char temp[40];

	strcat(temp, name);
	for (int i = 0; i < leng; i++) {
		if (temp[i] == ' ' && temp[i + 1] == ' ') {
			for (int j = i; j <= leng; j++) {
				temp[j] = temp[j + 1];			
			}
			leng--;
			i--;
		}
	}

	if (temp[0] = ' ') {
		for (int i = 0; i < leng; i ++)
			temp[i] = temp[i + 1];
	}
    return true;
}

/**
 * isValidUsername:
 * Check whether the given username is valid.
 */
bool isValidUsername(char *username) {
	int leng = strlen(username);

	//Check range
	if (leng > 20 || leng < 5)
    	return false;

    //Check special character	
    for (int i = 0; i < leng; i++) {
    	char c = username[i];

		if ( c < '0' || ( c > '9' && c < 'A' ) || ( c > 'Z' && c < 'a' ) || c > 'z' )
			return false;
	}
    	
    return true;
}

/**
 * isValidPassword:
 * Check whether the given password is valid.
 */
bool isValidPassword(char *password) {
	int leng = strlen(password);

	//Check range
	if (leng > 20 || leng < 6)
    	return false;

    //Check special character	
    for (int i = 0; i < leng; i++) {
    	char c = password[i];

		if ( ( c < '0' || ( c > '9' && c < 'A' ) || ( c > 'Z' && c < 'a' ) || c > 'z' ) && c != ' ' )
			return false;
	}
    	
    return true;
}

/**
 * userLogin:
 * Receive username and password.
 * If there is such account and right password, return 1.
 * If there is such account but wrong password, return 0.
 * If there is already an account with the given username, return -1.
 */
int userLogin(char *givenUsername, char *givenPassword) {
    User user = {};

    getUserByUsername(givenUsername, &user);
    if (user.username == NULL || user.password == NULL) {
        return -1;
    } else if (strcmp(user.password, givenPassword) != 0) {
        return 0;
    } else {
        return 1;
    }
}

/**
 * registerNewAccount:
 * Receive and insert the given user to file.
 * If fail to insert, return 0.
 * If insert successfully, return 1.
 */
int registerNewAccount(User user) {
    if (insertUser(user) != 1) {
        return 0; 
    } else {
        return 1;
    }
}

void leftTrim(char *name) {
    int leftIndex = 0;

    while (name[leftIndex] == ' ')
        leftIndex++;
    if (leftIndex > 0) 
        strcpy(&name[0], &name[leftIndex]);
}

void rightTrim(char *name) {
    int rightIndex = strlen(name) - 1;

    while (name[rightIndex] == ' ')
        rightIndex--;
    name[rightIndex + 1] = '\0';
}

void spaceTrim(char *name) {
    int i, x;
    for(i = x = 0; name[i]; ++i)
        if(!(name[i] == ' ') || (i > 0 && !(name[i-1] == ' ')))
            name[x++] = name[i];
    name[x] = '\0';
}

void modifyName(char *name) {
    leftTrim(name);
    rightTrim(name);
    spaceTrim(name);

    for (int i = 0; name[i]; ++i) {
        name[i] = tolower(name[i]);
        if ((i == 0) || (i > 0 && (name[i - 1] == ' ')))
            name[i] = toupper(name[i]);
    }
}

#endif
