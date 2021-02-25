#include <stdio.h>
#include "../header/Login.h"
#include "../header/LunarCalendar.h"
#include <stdbool.h>
#include <string.h>

int main() {
	char userTxtSign[128];
	char passTxtSign[128];
    

    printf("Username : " );
    scanf("%[^\n]", userTxtSign);
    fflush(stdin);
    printf("Password : " );
    scanf("%[^\n]", passTxtSign);
    fflush(stdin);

    bool isValidUsernameTxt = isValidUsername(userTxtSign);
    bool isValidPasswordTxt = isValidPassword(passTxtSign);

    if (!isValidUsernameTxt)
        printf("Wrong username");
    else if (!isValidPasswordTxt)
        printf("Wrong password");
    else {
        int status = userLogin(userTxtSign, passTxtSign);

        if (status == 0) {
            printf("Wrong password");
            //Do something if there is no such account
        } else if (status == -1) {
            printf("No account");
            //Do something if there is such account but wrong password
        } else {
            printf("Right account and password");
            //Do something if there is such account and right password
        }
    }

    // printf("Birthday : " );
    // scanf("%[^\n]", birthdayTxtSign);
    // fflush(stdin);

    // Date date = {};
    // parseDateOfBirth(birthdayTxtSign, &date);

    // printf("%d / %d / %d", date.day, date.month, date.year);
    return 0;
}