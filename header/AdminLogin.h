#ifndef ADMIN_LOGIN_H
#define ADMIN_LOGIN_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "LunarCalendar.h"
#include "UserLogin.h"
#include "../model/UserModel.h"

/**
 * Prototypes
 */
void getAdminByID(int id, Admin *fetchedAdmin);
void getAdminByUsername(char *username, Admin *fetchedAdmin);
void getAdminByName(char *name, Admin *fetchedAdmin);
int getLastAdminID();
bool isAdminIDExisted(int id);
bool isAdminUsername(char *username);
int insertAdmin(Admin admin);
int adminLogin(char *username, char *password);
int updateUsername(int id, char newUsername[]);
int updatePassword(int id, char *newPassword);
int deleteUser(User *user);

FILE *adminDataFile;
FILE *tempDataFile;
const char ADMIN_FILE[] = "data/admins_data.dat";
const char TEMP_FILE[] = "data/temp_data.dat";

/**
 * getAdminByID:
 * Search for Admin with the given id. Then get information and store into fetchedAdmin.
 * If not found, set Admin's information to default value (0 and NULL).
 */
void getAdminByID(int id, Admin *fetchedAdmin) {
    adminDataFile = fopen(ADMIN_FILE, "r");

    fetchedAdmin->id = 0;
    fetchedAdmin->name = NULL;
    fetchedAdmin->username = NULL;
    fetchedAdmin->password = NULL;

    if (adminDataFile != NULL) {
        char fetchedInformation[1000];

        /*
         * Loop to get every line in file
         */
        while (fgets(fetchedInformation, 1000, adminDataFile) != NULL) {
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
                //Store user's info into struct
                fetchedAdmin->id = (int) strtol(tokens[0], &tempPointer, 10);
                fetchedAdmin->name = tokens[1];
                fetchedAdmin->username = tokens[2];
                fetchedAdmin->password = tokens[3];

                break;
            }
        }

        fclose(adminDataFile);
    } else {
        printf("Error!! File is not existed.\n");
    }
}

/**
 * getAdminByName:
 * Search for Admin with the given name. Then get information and store into fetchedAdmin.
 * If not found, set Admin's information to default value (0 and NULL).
 */
void getAdminByName(char *givenName, Admin *fetchedAdmin) {
    adminDataFile = fopen(ADMIN_FILE, "r");

    fetchedAdmin->id = 0;
    fetchedAdmin->name = NULL;
    fetchedAdmin->username = NULL;
    fetchedAdmin->password = NULL;

    if (adminDataFile != NULL) {
        char fetchedInformation[1000];

        /*
         * Loop to get every line in file
         */
        while (fgets(fetchedInformation, 1000, adminDataFile) != NULL) {
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

            //Check whether file has the given name or not
            if (strcmp(tokens[1], givenName) == 0) {
                //Store user's info into struct
                fetchedAdmin->id = (int) strtol(tokens[0], &tempPointer, 10);
                fetchedAdmin->name = tokens[1];
                fetchedAdmin->username = tokens[2];
                fetchedAdmin->password = tokens[3];

                break;
            }
        }

        fclose(adminDataFile);
    } else {
        printf("Error!! File is not existed.\n");
    }
}

/**
 * getAdminByUsername:
 * Search for Admin with the given username. Then get information and store into fetchedAdmin.
 * If not found, set Admin's information to default value (0 and NULL).
 */
void getAdminByUsername(char *givenUsername, Admin *fetchedAdmin) {
    adminDataFile = fopen(ADMIN_FILE, "r");

    fetchedAdmin->id = 0;
    fetchedAdmin->name = NULL;
    fetchedAdmin->username = NULL;
    fetchedAdmin->password = NULL;

    if (adminDataFile != NULL) {
        char fetchedInformation[1000];

        /*
         * Loop to get every line in file
         */
        while (fgets(fetchedInformation, 1000, adminDataFile) != NULL) {
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
                //Store user's info into struct
                fetchedAdmin->id = (int) strtol(tokens[0], &tempPointer, 10);
                fetchedAdmin->name = tokens[1];
                fetchedAdmin->username = tokens[2];
                fetchedAdmin->password = tokens[3];

                break;
            }
        }

        fclose(adminDataFile);
    } else {
        printf("Error!! File is not existed.\n");
    }
}

/**
 * getLastAminID:
 * Return the last Admin's ID in the file. If failed, return -1.
 */
int getLastAdminID() {
    adminDataFile = fopen(ADMIN_FILE, "r");
    int lastID = 0;

    if (adminDataFile != NULL) {
        char fetchedInformation[1000];

        /*
         * Loop to get every line in file
         */
        while (fgets(fetchedInformation, 1000, adminDataFile) != NULL) {
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

        fclose(adminDataFile);
        return lastID;
    } else {
        printf("Error!! File is not existed.\n");
        return -1;
    }
}

/**
 * isAdminIDExisted:
 * Check whether a Admin with the given ID exists.
 */
bool isAdminIDExisted(int id) {
    if (id < 1)
        return false;

    adminDataFile = fopen(ADMIN_FILE, "r");

    if (adminDataFile != NULL) {
        char fetchedInformation[1000];

        /*
         * Loop to get every line in file
         */
        while (fgets(fetchedInformation, 1000, adminDataFile) != NULL) {
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
                fclose(adminDataFile);
                return true;
            }
        }

        fclose(adminDataFile);
        return false;
    } else {
        printf("Error!! File is not existed.\n");
    }
}

/**
 * isAdminUsernameExisted:
 * Check whether a Admin with the given Username exists.
 */
bool isAdminUsernameExisted(char *username) {
    adminDataFile = fopen(ADMIN_FILE, "r");

    if (adminDataFile != NULL) {
        char fetchedInformation[1000];

        /*
         * Loop to get every line in file
         */
        while (fgets(fetchedInformation, 1000, adminDataFile) != NULL) {
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
                fclose(adminDataFile);
                return true;
            }
        }

        fclose(adminDataFile);
        return false;
    } else {
        printf("Error!! File is not existed.\n");
    }
}

/**
 * insertAdmin:
 * Append a Admin to the end of file. 
 * If insert successfully, return 1.
 * If there is already a user with the same username, return 0. 
 * If the parameter user is NULL, return -1.
 */
int insertAdmin(Admin admin) {
    if (admin.name == NULL || admin.username == NULL || admin.password == NULL) {
        return -1;
    }

    if (isAdminUsernameExisted(admin.username)) {
        return 0;
    } else {
        admin.id = getLastAdminID() + 1;

        adminDataFile = fopen(ADMIN_FILE, "a");
        
        fprintf(userDataFile, "%d-%s-%s-%s\n", admin.id, admin.name, 
                admin.username, admin.password);

        fclose(adminDataFile);        
        return 1;
    }
}

/**
 * adminLogin:
 * Receive username and password.
 * If there is such account and right password, return 1.
 * If there is such account but wrong password, return 0.
 * If there is already an account with the given username, return -1.
 */
int adminLogin(char *givenUsername, char *givenPassword) {
    Admin admin = {};

    getAdminByUsername(givenUsername, &admin);
    if (admin.username == NULL || admin.password == NULL) {
        return -1;
    } else if (strcmp(admin.password, givenPassword) != 0) {
        return 0;
    } else {
        return 1;
    }
}

/**
 * updateUsername:
 * Change username of the User with the given username to the new username.
 * After updating, the User pointer may point to some undefined data. You need to get user again.
 * If update successfully, return 1.
 * If there is no such account, return 0.
 * If there are error in update process, return -1.
 */
int updateUsername(int id, char *newUsername) {
    if (id == 0) {
        return 0;
    }
    
    User user = {};
    getUserByID(id, &user);
    if (user.username == NULL) {
        return 0;
    }

    int oldID;
    char oldName[1000];
    char oldPassword[1000];

    oldID = user.id;
    strcpy(oldName, user.name);
    strcpy(oldPassword, user.password);

    /* 
     * Copy all data from the user file to a temp file.
     */
    tempDataFile = fopen(TEMP_FILE, "w");
    userDataFile = fopen(USER_FILE, "r");

    if (userDataFile != NULL) {
        char tempString[1000];
        
        while (fgets(tempString, 1000, userDataFile) != NULL) {
            fprintf(tempDataFile, "%s", tempString);
        }

        fclose(tempDataFile);
        fclose(userDataFile);
    } else {
        printf("Error!! File is not existed!!!\n");
        return -1;
    }

    /*
     * Transfer the data from the temp file to the user file. And update specific user's info.
     */
    tempDataFile = fopen(TEMP_FILE, "r");
    userDataFile = fopen(USER_FILE, "w");

    if (tempDataFile != NULL) {
        char tempString[1000];
        int count = 1;

        while (fgets(tempString, 1000, tempDataFile) != NULL) {
            if (count == user.id) {
                fprintf(userDataFile, "%d-%s-%s-%s\n", oldID, oldName, newUsername, oldPassword);
                count++;
            } else {
                fprintf(userDataFile, "%s", tempString);
                count++;
            }
        }

        fclose(tempDataFile);
        fclose(userDataFile);
        return 1;
    } else {
        printf("Error!! File is not existed!!!\n");
        return -1;
    }
}

/**
 * updatePassword:
 * Change password of the User with the given username to the new password.
 * After updating, the User pointer may point to some undefined data. You need to get user again.
 * If update successfully, return 1.
 * If there is no such account, return 0.
 * If there are error in update process, return -1.
 */
int updatePassword(int id, char *newPassword) {
    if (id == 0) {
        return 0;
    }
    
    User user = {};
    getUserByID(id, &user);
    if (user.username == NULL) {
        return 0;
    }

    int oldID;
    char oldName[1000];
    char oldUsername[1000];

    oldID = user.id;
    strcpy(oldName, user.name);
    strcpy(oldUsername, user.username);

    /* 
     * Copy all data from the user file to a temp file.
     */
    tempDataFile = fopen(TEMP_FILE, "w");
    userDataFile = fopen(USER_FILE, "r");

    if (userDataFile != NULL) {
        char tempString[1000];
        
        while (fgets(tempString, 1000, userDataFile) != NULL) {
            fprintf(tempDataFile, "%s", tempString);
        }

        fclose(tempDataFile);
        fclose(userDataFile);
    } else {
        printf("Error!! File is not existed!!!\n");
        return -1;
    }

    /*
     * Transfer the data from the temp file to the user file. And update specific user's info.
     */
    tempDataFile = fopen(TEMP_FILE, "r");
    userDataFile = fopen(USER_FILE, "w");

    if (tempDataFile != NULL) {
        char tempString[1000];
        int count = 1;

        while (fgets(tempString, 1000, tempDataFile) != NULL) {
            if (count == user.id) {
                fprintf(userDataFile, "%d-%s-%s-%s\n", oldID, oldName, oldUsername, newPassword);
                count++;
            } else {
                fprintf(userDataFile, "%s", tempString);
                count++;
            }
        }

        fclose(tempDataFile);
        fclose(userDataFile);
        return 1;
    } else {
        printf("Error!! File is not existed!!!\n");
        return -1;
    }
}

/**
 * deleteUser:
 * Remove the given User and set the User's data to default value (0 and NULL).
 * If delete successfully, return 1.
 * If there is no such account, return 0.
 * If there are error in delete process, return -1.
 */
int deleteUser(User *user) {
    if (user->username == NULL) {
        return 0;
    }

    /* 
     * Copy all data from the user file to a temp file.
     */
    tempDataFile = fopen(TEMP_FILE, "w");
    userDataFile = fopen(USER_FILE, "r");

    if (userDataFile != NULL) {

        char tempString[1000];
        int count = 1;

        while (fgets(tempString, 1000, userDataFile) != NULL) {
            fprintf(tempDataFile, "%s", tempString);
        }

        fclose(tempDataFile);
        fclose(userDataFile);

        /* 
         * Transfer data to the user file again. And skip transfering the specific user.
         */
        tempDataFile = fopen(TEMP_FILE, "r");
        userDataFile = fopen(USER_FILE, "w");

        if (tempDataFile == NULL) {
            printf("Error!! File is not existed!!!\n");
            return -1;
        } else {
            while (fgets(tempString, 1000, tempDataFile) != NULL) {
                if (count == user->id) {
                    count++;
                } else if (count < user->id) {
                    fprintf(userDataFile, "%s", tempString);
                    count++;
                } else {
                    tempString[0] = tempString[0] - 1;
                    fprintf(userDataFile, "%s", tempString);
                    count++;
                }
            }

            user->id = 0;
            user->name = NULL;
            user->username = NULL;
            user->password = NULL;

            fclose(tempDataFile);
            fclose(userDataFile);
            return 1;
        }   
    } else {
        printf("Error!! File is not existed!!!\n");
        return -1;
    }
}

#endif
