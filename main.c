#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "header/solarLogic.c"
#include <string.h>
#include "header/UserLogin.h"
#include "header/AdminLogin.h"
#include "header/LunarCalendar.h"
#include "header/EventLogic.h"
static int curImportant, curReminder, curWork, curEvent;
static int curReviewWindow;
int currentMonth;
int currentYear;
char currUserName[128];
char manageUsername[128];
//Calendar
GtkWidget *windowCalendar;
GtkWidget *grid;
GtkWidget *yearLbl;
GtkWidget *yearLblSmall;
GtkWidget *monthLblSmall;
GtkWidget *lbCurrentDate;
GtkWidget *lbUserName;
GtkWidget *solarDay[37];
GtkWidget *solarSmallDay[37];
GtkWidget *lunarDay[37];
GtkWidget *weekLbl[10];
GtkWidget *holidayLbl[37];
//variable for ID
User user = {};
// Login
GtkWidget *windowLogin;
GtkEntry *userEntryLogin;
GtkEntry *passEntryLogin;
GtkWidget *lbUserCorLogin;
GtkWidget *lbPassCorLogin;
// Admin
GtkWidget *windowLoginAdmin;
GtkEntry *userEntryAdmin;
GtkEntry *passEntryAdmin;
GtkWidget *lbUserCorAdmin;
GtkWidget *lbPassCorAdmin;
// liststore to save user
GtkListStore *liststoreManage;
GtkTreeView *treeViewManage;
GtkTreeViewColumn *colID;
GtkTreeViewColumn *colUsername;
GtkTreeViewColumn *colName;
GtkTreeViewColumn *colPass;
GtkCellRenderer *renderID;
GtkCellRenderer *renderUsername;
GtkCellRenderer *renderName;
GtkCellRenderer *renderPass;
GtkTreeSelection *selection;
// Update window
GtkWidget *windowUpdate;
GtkEntry *userEntryUpdate;
GtkEntry *passEntryUpdate;
GtkEntry *confPassEntryUpdate;
GtkWidget *lbUserCorUpdate;
GtkWidget *lbPassCorUpdate;
GtkWidget *lbConfCorUpdate;
//window manage
GtkWidget *windowManage;
// successfull dialog
GtkWidget *dialogSuccess;
GtkWidget *dialogSuccessUpdate;
// Sign Up
GtkWidget *windowSign;
GtkEntry *nameEntrySign;
GtkEntry *userEntrySign;
GtkEntry *passEntrySign;
GtkEntry *confPassEntrySign;
GtkWidget *lbNameCor;
GtkWidget *lbUserCor;
GtkWidget *lbPassCor;
GtkWidget *lbConfCor;
//Create event
GtkWidget *popover3;
GtkWidget *cancelBtn;
GtkWidget *popupCalendar;
GtkWidget *evenTitleInput;
GtkWidget *eventContentInput;
GtkWidget *dateEventInput;
GtkWidget *createBtn;
GtkTextBuffer *createEventBuf;
int importantCheck, reminderCheck, workCheck;
//Review event
GtkWidget *reviewEvent;
GtkWidget *importantTitle;
GtkWidget *importantLbl;
GtkWidget *reminderTitle;
GtkWidget *reminderLbl;
GtkWidget *workTitle;
GtkWidget *workLbl;
GtkWidget *dateLbl1, *dateLbl2, *dateLbl3;
//Show evnt on main window
GtkWidget *lbImportant[37];
GtkWidget *lbReminder[37];
GtkWidget *lbWork[37];
GtkWidget *lbTask[37];
int curReminderID, curImportantID, curWorkID;
int windowImportant, windowReminder, windowWork;
int darkModeState;

// File save id
FILE *saveIDPointer;
const char SAVE_ID[] = "data/SaveID.dat";
//=============Load css function=================
static void load_css(void)
{
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;
    ///***
    const gchar *css_style_file = "gui/style.css";
    GFile *css_fp = g_file_new_for_path (css_style_file);
    GError *error = 0;
    ///***
    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen (display);
    ///***
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_file (provider, css_fp, &error);
    ///***
    g_object_unref (provider);
 }
 static void load_css_dark_mode(void)
{
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;
    ///***
    const gchar *css_style_file = "gui/style-darkmode.css";
    GFile *css_fp = g_file_new_for_path (css_style_file);
    GError *error = 0;
    ///***
    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen (display);
    ///***
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_file (provider, css_fp, &error);
    ///***
    g_object_unref (provider);
 }

//====================
static void showEventReview(int currentDay) {
    curReminderID = 0, curImportantID = 0, curWorkID = 0;
    curReviewWindow = currentDay;
    if (currentDay == 0) {
        gtk_label_set_text(GTK_LABEL(dateLbl1), "");
        gtk_label_set_text(GTK_LABEL(dateLbl2), "");
        gtk_label_set_text(GTK_LABEL(dateLbl3), "");

    } else {
        char dateStr[12], buf[5];
        sprintf(buf, "%d", currentDay);
        strcat(dateStr, buf);
        strcat(dateStr, "/");
        sprintf(buf, "%d", currentMonth);
        strcat(dateStr, buf);
        strcat(dateStr, "/");
        sprintf(buf, "%d", currentYear);
        strcat(dateStr, buf);
        for (int i = 0; i < strlen(dateStr); i++) {
            if (dateStr[i] < '\n' || dateStr[i] > '~') {
                dateStr[i] = ' ';
            }
        }

        gtk_label_set_text(GTK_LABEL(dateLbl1), dateStr);
        gtk_label_set_text(GTK_LABEL(dateLbl2), dateStr);
        gtk_label_set_text(GTK_LABEL(dateLbl3), dateStr);

    }
    if (currentDay != 0) {
        char* strTask1 = getTaskbyDate(3, currentDay, currentMonth, currentYear);
        int i = 0;
        char *tokens[40];
        char *token;
        token = strtok(strTask1, "-");
		while (token != NULL) {
            tokens[i] = token;
            i++;
            token = strtok(NULL, "-");
        }
        int importantList[10], reminderList[10], workList[10];
        int maxImportant = 0, maxReminder = 0, maxWork = 0;
        for (int tokenIndex = 1; tokenIndex < i; tokenIndex+=3) {
            if (strcmp(tokens[tokenIndex], "3") == 0) {
                maxImportant++;
                importantList[maxImportant] = tokenIndex;
            } else if (strcmp(tokens[tokenIndex], "1") == 0) {
                maxReminder++;
                reminderList[maxReminder] = tokenIndex;
            } else if (strcmp(tokens[tokenIndex], "2") == 0) {
                maxWork++;
                workList[maxWork] = tokenIndex;
            }
        }
        //show important event
        if (maxImportant > 0) {
            if (curImportant == 0) curImportant = maxImportant;
            if (curImportant > maxImportant) curImportant = 1;
            curImportantID = importantList[curImportant];
            gtk_label_set_text(GTK_LABEL(importantTitle), tokens[importantList[curImportant] + 1]);
            char* tempoStr = tokens[importantList[curImportant] + 2];
            for (int j = 0; j <= strlen(tempoStr); j++) {
                if (tempoStr[j] == '~') {
                    tempoStr[j] = '\n';
                }
            }
            gtk_label_set_text(GTK_LABEL(importantLbl), tempoStr);
        } else {
            gtk_label_set_text(GTK_LABEL(importantTitle), "There is no important event today");
            gtk_label_set_text(GTK_LABEL(importantLbl), "");
        }
        //show reminder event
        if (maxReminder > 0) {
            if (curReminder == 0) curReminder = maxReminder;
            if (curReminder > maxReminder) curReminder = 1;
            curReminderID = reminderList[curReminder];
            gtk_label_set_text(GTK_LABEL(reminderTitle), tokens[reminderList[curReminder] + 1]);
            char* tempoStr = tokens[reminderList[curReminder] + 2];
            for (int j = 0; j <= strlen(tempoStr); j++) {
                if (tempoStr[j] == '~') {
                    tempoStr[j] = '\n';
                }
            }
            gtk_label_set_text(GTK_LABEL(reminderLbl), tempoStr);
        } else {
            gtk_label_set_text(GTK_LABEL(reminderTitle), "There is no reminder event today");
            gtk_label_set_text(GTK_LABEL(reminderLbl), "");
        }
        //show work event
        if (maxWork > 0) {
            if (curWork == 0) curWork = maxWork;
            if (curWork > maxWork) curWork = 1;
            curWorkID = workList[curWork];
            gtk_label_set_text(GTK_LABEL(workTitle), tokens[workList[curWork] + 1]);
            char* tempoStr = tokens[workList[curWork] + 2];
            for (int j = 0; j <= strlen(tempoStr); j++) {
                if (tempoStr[j] == '~') {
                    tempoStr[j] = '\n';
                }
            }
            gtk_label_set_text(GTK_LABEL(workLbl), tempoStr);
        } else {
            gtk_label_set_text(GTK_LABEL(workTitle), "There is no work event today");
            gtk_label_set_text(GTK_LABEL(workLbl), "");
        }



    } else {
        gtk_label_set_text(GTK_LABEL(importantTitle), "Empty cell");
        gtk_label_set_text(GTK_LABEL(reminderTitle), "Empty cell");
        gtk_label_set_text(GTK_LABEL(workTitle), "Empty cell");
        gtk_label_set_text(GTK_LABEL(importantLbl), "");
        gtk_label_set_text(GTK_LABEL(reminderLbl), "");
        gtk_label_set_text(GTK_LABEL(workLbl), "");

    }

}

static void makeCalendar()
{
    getVal(currentYear);

    int numEmptyDay = emptyDay[currentMonth - 1];
    int numDay = 1;
    int postDay = 1;
    //make day button
    for (int i = 1; i <= 35; i++)
    {
        gtk_label_set_text(GTK_LABEL(solarDay[i]), "");
        gtk_label_set_text(GTK_LABEL(solarSmallDay[i]), "");
        gtk_label_set_text(GTK_LABEL(lunarDay[i]), "");
        gtk_label_set_text(GTK_LABEL(lbImportant[i]), "");
        gtk_label_set_text(GTK_LABEL(lbReminder[i]), "");
        gtk_label_set_text(GTK_LABEL(lbWork[i]), "");
        gtk_label_set_text(GTK_LABEL(holidayLbl[i]), "");
    }
    LunarDate lnDate;
    for (int i = numEmptyDay + 1; i <= 35; i++)
    {
        if (numDay <= maxDay[currentMonth - 1])
        {
            gtk_label_set_text(GTK_LABEL(solarDay[i]), date[numDay]);
            gtk_label_set_text(GTK_LABEL(solarSmallDay[i]), date[numDay]);
            gtk_label_set_text(GTK_LABEL(holidayLbl[i]), solarHolidayHandler(numDay, currentMonth));
            lnDate = convertSolarToLunar(numDay, currentMonth, currentYear, 7.0);
            if (lunarHolidayHandler(lnDate.day, lnDate.month) != "")
                gtk_label_set_text(GTK_LABEL(holidayLbl[i]), lunarHolidayHandler(lnDate.day, lnDate.month));

            char str[] = "", strNum[10];
            sprintf(strNum, "%d", lnDate.day);
            strcat(str, strNum);
            sprintf(strNum, "%d ", lnDate.month);
            strcat(str, "/");
            strcat(str, strNum);
            gtk_label_set_text(GTK_LABEL(lunarDay[i]), str);
            //event handle
            char* strTask1 = getTaskbyDate(3, numDay, currentMonth, currentYear);
            int ii = 0;
            char *tokens[40];
            char *token;
            token = strtok(strTask1, "-");
            while (token != NULL) {
                tokens[ii] = token;
                ii++;
                token = strtok(NULL, "-");
            }
            int maxImportant = 0, maxReminder = 0, maxWork = 0;
            for (int tokenIndex = 1; tokenIndex < ii; tokenIndex+=3) {
                if (strcmp(tokens[tokenIndex], "3") == 0) {
                    maxImportant++;
                } else if (strcmp(tokens[tokenIndex], "1") == 0) {
                    maxReminder++;
                } else if (strcmp(tokens[tokenIndex], "2") == 0) {
                    maxWork++;
                }
            }
            if (maxImportant > 0 && windowImportant == 1)
                gtk_label_set_text(GTK_LABEL(lbImportant[i]), ".");
            if (maxReminder > 0 && windowReminder == 1)
                gtk_label_set_text(GTK_LABEL(lbReminder[i]), ".");
            if (maxWork > 0 && windowWork == 1)
                gtk_label_set_text(GTK_LABEL(lbWork[i]), ".");


            numDay++;

        }
    }
    while (numDay <= maxDay[currentMonth - 1])
    {
        gtk_label_set_text(GTK_LABEL(solarDay[postDay]), date[numDay]);
        gtk_label_set_text(GTK_LABEL(solarSmallDay[postDay]), date[numDay]);
        gtk_label_set_text(GTK_LABEL(holidayLbl[postDay]), solarHolidayHandler(numDay, currentMonth));
        lnDate = convertSolarToLunar(numDay, currentMonth, currentYear, 7.0);
        if (lunarHolidayHandler(lnDate.day, lnDate.month) != "")
            gtk_label_set_text(GTK_LABEL(holidayLbl[postDay]), lunarHolidayHandler(lnDate.day, lnDate.month));
        char str[] = "", strNum[10];
        sprintf(strNum, "%d", lnDate.day);
        strcat(str, strNum);
        sprintf(strNum, "%d", lnDate.month);
        strcat(str, "/");
        strcat(str, strNum);
        gtk_label_set_text(GTK_LABEL(lunarDay[postDay]), str);

        //event handle
        char* strTask1 = getTaskbyDate(3, numDay, currentMonth, currentYear);
        int ii = 0;
        char *tokens[40];
        char *token;
        token = strtok(strTask1, "-");
        while (token != NULL) {
            tokens[ii] = token;
            ii++;
            token = strtok(NULL, "-");
        }
        int maxImportant = 0, maxReminder = 0, maxWork = 0;
        for (int tokenIndex = 1; tokenIndex < ii; tokenIndex+=3) {
            if (strcmp(tokens[tokenIndex], "3") == 0) {
                maxImportant++;
            } else if (strcmp(tokens[tokenIndex], "1") == 0) {
                maxReminder++;
            } else if (strcmp(tokens[tokenIndex], "2") == 0) {
                maxWork++;
            }
        }
        if (maxImportant > 0)
            gtk_label_set_text(GTK_LABEL(lbImportant[postDay]), ".");
        if (maxReminder > 0)
            gtk_label_set_text(GTK_LABEL(lbReminder[postDay]), ".");
        if (maxWork > 0)
            gtk_label_set_text(GTK_LABEL(lbWork[postDay]), ".");

        numDay++;
        postDay++;
    }

}
void showCalendarWindow(int argc, char *argv[])
{
    GtkBuilder *builderCalendar;

    gtk_init(&argc, &argv);
    builderCalendar = gtk_builder_new();
    gtk_builder_add_from_file(builderCalendar, "gui/CalendarlayoutYes.glade", NULL);

    windowCalendar = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "window_main"));
    yearLbl = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "year"));
    yearLblSmall = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "yearSmall"));
    monthLblSmall = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "monthSmall"));
    lbCurrentDate = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "lbCurrentDate"));
    lbUserName = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "lbUserName"));
    //widget for create event
    popupCalendar = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "popupCalendar"));
    dateEventInput = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "dateEventInput"));
    evenTitleInput = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "evenTitleInput"));
    eventContentInput = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "eventContentInput"));
    createBtn = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "createBtn"));
    createEventBuf = GTK_TEXT_BUFFER(gtk_builder_get_object(builderCalendar, "createEventBuf"));
    popover3 = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "popover3"));
    cancelBtn = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "cancelBtn"));

    //init state of check buttons
    importantCheck = 0, reminderCheck = 0, workCheck = 1;
    //init state of window button
    windowImportant = 1, windowReminder = 1, windowWork = 1;
    //widget for review event
    reviewEvent = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "reviewEvent"));
    dateLbl1 = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "dateLbl1"));
    dateLbl2 = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "dateLbl2"));
    dateLbl3 = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "dateLbl3"));
    importantTitle = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "importantTitle"));
    importantLbl = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "importantLbl"));
    reminderTitle = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "reminderTitle"));
    reminderLbl = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "reminderLbl"));
    workTitle = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "workTitle"));
    workLbl = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "workLbl"));
    // set text username label
    if(user.id != 0){
        g_print("%s", currUserName);
        gtk_label_set_text(GTK_LABEL(lbUserName), currUserName);
    }

    for (int i = 1; i <= 35; i++) {
        char ID[3];
        sprintf(ID, "%d", i);
        //g_print("%s", solarID);
        //solarDay[i] = GTK_WIDGET(gtk_builder_get_object(builderCalendar, solarID));
        char lunarID[] = "lunar";
        strcat(lunarID, ID);
        lunarDay[i] = GTK_WIDGET(gtk_builder_get_object(builderCalendar, lunarID));
    }
    for (int i = 1; i <= 35; i++) {
        char ID[3];
        sprintf(ID, "%d", i);
        char solarID[] = "lb";
        strcat(solarID, ID);
        solarDay[i] = GTK_WIDGET(gtk_builder_get_object(builderCalendar, solarID));
    }
    for (int i = 1; i <= 35; i++) {
        char ID[3];
        sprintf(ID, "%d", i);
        char solarIDSmall[] = "lbSmall";
        strcat(solarIDSmall, ID);
        solarSmallDay[i] = GTK_WIDGET(gtk_builder_get_object(builderCalendar, solarIDSmall));
    }
    for (int i = 1; i <= 35; i++) {
        char ID[3];
        sprintf(ID, "%d", i);
        char importantIDStr[] = "lbImportant";
        strcat(importantIDStr, ID);
        lbImportant[i] = GTK_WIDGET(gtk_builder_get_object(builderCalendar, importantIDStr));
    }
    for (int i = 1; i <= 35; i++) {
        char ID[3];
        sprintf(ID, "%d", i);
        char reminderIDstr[] = "lbReminder";
        strcat(reminderIDstr, ID);
        lbReminder[i] = GTK_WIDGET(gtk_builder_get_object(builderCalendar, reminderIDstr));
    }
    for (int i = 1; i <= 35; i++) {
        char ID[3];
        sprintf(ID, "%d", i);
        char workIDStr[] = "lbWork";
        strcat(workIDStr, ID);
        lbWork[i] = GTK_WIDGET(gtk_builder_get_object(builderCalendar, workIDStr));
    }
    for (int i = 1; i <= 35; i++) {
        char ID[3];
        sprintf(ID, "%d", i);
        char taskIDStr[] = "lbTask";
        strcat(taskIDStr, ID);
        lbTask[i] = GTK_WIDGET(gtk_builder_get_object(builderCalendar, taskIDStr));
        gtk_label_set_text(GTK_LABEL(lbTask[i]), "");
    }
    for (int i = 1; i <= 35; i++) {
        char ID[3];
        sprintf(ID, "%d", i);
        char holidayIDStr[] = "holiday";
        strcat(holidayIDStr, ID);
        holidayLbl[i] = GTK_WIDGET(gtk_builder_get_object(builderCalendar, holidayIDStr));
    }


    // current day
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    currentYear = tm.tm_year + 1900;
    char strYear[4];
    sprintf(strYear, "%d", currentYear);
    currentMonth = tm.tm_mon + 1;
    gtk_label_set_text(GTK_LABEL(yearLbl), strYear);
    gtk_label_set_text(GTK_LABEL(yearLblSmall), strYear);
    gtk_label_set_text(GTK_LABEL(monthLblSmall), (char*)months[tm.tm_mon]);

    char strd[] = "", strNumD[5];
    strcat(strd, (char*)months[tm.tm_mon]);
    strcat(strd, strNumD);
    sprintf(strNumD, "%d", tm.tm_mday);
    strcat(strd, ", ");
    strcat(strd, strNumD);
    gtk_label_set_text(GTK_LABEL(lbCurrentDate), strd);

    makeCalendar();

    gtk_builder_connect_signals(builderCalendar, NULL);

    g_object_unref(builderCalendar);

    gtk_widget_show(windowCalendar);
    if (darkModeState == 0) {
        load_css();
    } else {
        load_css_dark_mode();
    }

    gtk_main();
}
void showLoginWindow(int argc, char *argv[])
{
    GtkBuilder *builderLogin;

    gtk_init(&argc, &argv);

    builderLogin = gtk_builder_new();
    gtk_builder_add_from_file (builderLogin, "gui/calendarLogin.glade", NULL);

    windowLogin = GTK_WIDGET(gtk_builder_get_object(builderLogin, "window_login"));
    userEntryLogin = GTK_ENTRY(gtk_builder_get_object(builderLogin, "userEntry"));
    passEntryLogin = GTK_ENTRY(gtk_builder_get_object(builderLogin, "passEntry"));
    lbUserCorLogin = GTK_WIDGET(gtk_builder_get_object(builderLogin, "lbUserCor"));
    lbPassCorLogin = GTK_WIDGET(gtk_builder_get_object(builderLogin, "lbPassCor"));

    gtk_builder_connect_signals(builderLogin, NULL);

    g_object_unref(builderLogin);

    gtk_widget_show(windowLogin);
    gtk_main();
}
void showSignupWindow(int argc, char *argv[])
{

    GtkBuilder *builderSign;
    gtk_init(&argc, &argv);

    builderSign = gtk_builder_new();
    gtk_builder_add_from_file (builderSign, "gui/calendarSign.glade", NULL);

    // GET WIFGET FROM BUILDER
    windowSign = GTK_WIDGET(gtk_builder_get_object(builderSign, "window_sig"));
    nameEntrySign = GTK_ENTRY(gtk_builder_get_object(builderSign, "nameEntry"));
    userEntrySign = GTK_ENTRY(gtk_builder_get_object(builderSign, "userEntry"));
    passEntrySign = GTK_ENTRY(gtk_builder_get_object(builderSign, "passEntry"));
    confPassEntrySign = GTK_ENTRY(gtk_builder_get_object(builderSign, "confPassEntry"));
    dialogSuccess = GTK_WIDGET(gtk_builder_get_object(builderSign, "dialogSuccess"));

    lbNameCor = GTK_WIDGET(gtk_builder_get_object(builderSign, "lbNameCor"));
    lbUserCor = GTK_WIDGET(gtk_builder_get_object(builderSign, "lbUserCor"));
    lbPassCor = GTK_WIDGET(gtk_builder_get_object(builderSign, "lbPassCor"));
    lbConfCor = GTK_WIDGET(gtk_builder_get_object(builderSign, "lbConfCor"));

    gtk_builder_connect_signals(builderSign, NULL);

    g_object_unref(builderSign);

    gtk_widget_show(windowSign);
    gtk_main();
}

void showLoginAdminWindow(int argc, char *argv[])
{
    GtkBuilder *builderLoginAdmin;

    gtk_init(&argc, &argv);

    builderLoginAdmin = gtk_builder_new();
    gtk_builder_add_from_file (builderLoginAdmin, "gui/calendarLoginAdmin.glade", NULL);
    userEntryAdmin = GTK_ENTRY(gtk_builder_get_object(builderLoginAdmin, "userEntryAdmin"));
    passEntryAdmin = GTK_ENTRY(gtk_builder_get_object(builderLoginAdmin, "passwordEntryAdmin"));
    lbUserCorAdmin = GTK_WIDGET(gtk_builder_get_object(builderLoginAdmin, "lbUserCor"));
    lbPassCorAdmin = GTK_WIDGET(gtk_builder_get_object(builderLoginAdmin, "lbPassCor"));

    windowLoginAdmin = GTK_WIDGET(gtk_builder_get_object(builderLoginAdmin, "window_login_Admin"));
    gtk_builder_connect_signals(builderLoginAdmin, NULL);

    g_object_unref(builderLoginAdmin);

    gtk_widget_show(windowLoginAdmin);
    gtk_main();
}
void showManageUserWindow(int argc, char *argv[])
{
    GtkBuilder *builderManage;
    GtkTreeIter iter;
    User userGet = {};
    int numUserID = getLastUserID();

    gtk_init(&argc, &argv);

    builderManage = gtk_builder_new();
    gtk_builder_add_from_file (builderManage, "gui/CalendarManageUser.glade", NULL);

    windowManage = GTK_WIDGET(gtk_builder_get_object(builderManage, "window_manage"));
    liststoreManage = GTK_LIST_STORE(gtk_builder_get_object(builderManage, "liststoreManage"));
    treeViewManage = GTK_TREE_VIEW(gtk_builder_get_object(builderManage, "treeViewManage"));
    colID = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builderManage, "colID"));
    colUsername = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builderManage, "colUsername"));
    colName = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builderManage, "colName"));
    colPass = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builderManage, "colPass"));
    renderID = GTK_CELL_RENDERER(gtk_builder_get_object(builderManage, "renderID"));
    renderUsername = GTK_CELL_RENDERER(gtk_builder_get_object(builderManage, "renderUsername"));
    renderName = GTK_CELL_RENDERER(gtk_builder_get_object(builderManage, "renderName"));
    renderPass = GTK_CELL_RENDERER(gtk_builder_get_object(builderManage, "renderPass"));
    selection = GTK_TREE_SELECTION(gtk_builder_get_object(builderManage, "selection"));

    gtk_tree_view_column_add_attribute(colID, renderID, "text", 0);
    gtk_tree_view_column_add_attribute(colUsername, renderUsername, "text", 1);
    gtk_tree_view_column_add_attribute(colName, renderName, "text", 2);
    gtk_tree_view_column_add_attribute(colPass, renderPass, "text", 3);

    char txtID[128];
    char txtUsername[128];
    char txtName[128];
    char txtPass[128];
    for(int i = 1; i <= numUserID; i++)
    {
        getUserByID(i, &userGet);
        sprintf(txtID, "%d", userGet.id);
        sprintf(txtUsername, "%s", userGet.username);
        sprintf(txtName, "%s", userGet.name);
        sprintf(txtPass, "%s", userGet.password);
        gtk_list_store_append(liststoreManage, &iter);
        gtk_list_store_set (liststoreManage, &iter, 0, txtID, -1);
        gtk_list_store_set (liststoreManage, &iter, 1, txtUsername, -1);
        gtk_list_store_set (liststoreManage, &iter, 2, txtName, -1);
        gtk_list_store_set (liststoreManage, &iter, 3, txtPass, -1);
    }
    gtk_builder_connect_signals(builderManage, NULL);

    g_object_unref(builderManage);

    gtk_widget_show(windowManage);
    gtk_main();
}

void showUpdateWindow(int argc, char *argv[])
{
    GtkBuilder *builderUpdate;

    gtk_init(&argc, &argv);

    builderUpdate = gtk_builder_new();
    gtk_builder_add_from_file (builderUpdate, "gui/calendarUpdateUser.glade", NULL);

    windowUpdate = GTK_WIDGET(gtk_builder_get_object(builderUpdate, "window_update"));
    userEntryUpdate = GTK_ENTRY(gtk_builder_get_object(builderUpdate, "userEntryUpdate"));
    passEntryUpdate = GTK_ENTRY(gtk_builder_get_object(builderUpdate, "passEntryUpdate"));
    confPassEntryUpdate = GTK_ENTRY(gtk_builder_get_object(builderUpdate, "confPassEntryUpdate"));
    lbUserCorUpdate = GTK_WIDGET(gtk_builder_get_object(builderUpdate, "lbUserCorUpdate"));
    lbPassCorUpdate = GTK_WIDGET(gtk_builder_get_object(builderUpdate, "lbPassCorUpdate"));
    lbConfCorUpdate = GTK_WIDGET(gtk_builder_get_object(builderUpdate, "lbConfCorUpdate"));
    dialogSuccessUpdate = GTK_WIDGET(gtk_builder_get_object(builderUpdate, "dialogSuccessUpdate"));


    // User variable
    User userUpdate = {};
    char userUpdateTxt[128];
    char passUpdateTxt[128];


    //get user by id
    getUserByID(user.id, &userUpdate);

    sprintf(userUpdateTxt, "%s", userUpdate.username);
    sprintf(passUpdateTxt, "%s", userUpdate.password);

    gtk_entry_set_text(userEntryUpdate, userUpdateTxt);
    gtk_entry_set_text(passEntryUpdate, passUpdateTxt);
    gtk_entry_set_text(confPassEntryUpdate, passUpdateTxt);
   

    
    gtk_builder_connect_signals(builderUpdate, NULL);

    g_object_unref(builderUpdate);

    gtk_widget_show(windowUpdate);
    gtk_main();
}
int main(int argc, char *argv[])
{
    saveIDPointer = fopen(SAVE_ID, "r");
    fscanf(saveIDPointer, "%d", &user.id);
    getUserByID(user.id, &user);
    sprintf(currUserName, "%s", user.username);
    fclose(saveIDPointer);
    showCalendarWindow(argc, argv);
    return 0;
}

G_MODULE_EXPORT void on_window_calendar_destroy()
{
    gtk_main_quit();
}
G_MODULE_EXPORT void on_CalendarManageUserWindow_destroy()
{
    gtk_main_quit();
}

G_MODULE_EXPORT void on_downYear_clicked(int argc, char *argv[])
{
    currentYear--;
    char strYear[10];
    sprintf(strYear, "%d", currentYear);
    gtk_label_set_text(GTK_LABEL(yearLbl), strYear);
    gtk_label_set_text(GTK_LABEL(yearLblSmall), strYear);
    makeCalendar();
}
G_MODULE_EXPORT void on_upYear_clicked(int argc, char *argv[])
{
    currentYear++;
    char strYear[10];
    sprintf(strYear, "%d", currentYear);
    gtk_label_set_text(GTK_LABEL(yearLbl), strYear);
    gtk_label_set_text(GTK_LABEL(yearLblSmall), strYear);
    makeCalendar();
}
G_MODULE_EXPORT void on_downMonth_clicked(int argc, char *argv[])
{
    if (currentMonth == 1) {
        currentMonth = 12;
        currentYear--;
        char strYear[10];
        sprintf(strYear, "%d", currentYear);
        gtk_label_set_text(GTK_LABEL(yearLbl), strYear);
        gtk_label_set_text(GTK_LABEL(yearLblSmall), strYear);

    } else {
        currentMonth--;
    }
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char strd[] = "", strNumD[10];
    strcat(strd, (char*)months[currentMonth - 1]);
    strcat(strd, strNumD);
    sprintf(strNumD, "%d", tm.tm_mday);
    strcat(strd, ", ");
    strcat(strd, strNumD);
    gtk_label_set_text(GTK_LABEL(lbCurrentDate), strd);
    gtk_label_set_text(GTK_LABEL(monthLblSmall), (char*)months[currentMonth - 1]);
    makeCalendar();
}
G_MODULE_EXPORT void on_upMonth_clicked(int argc, char *argv[])
{
    if (currentMonth == 12) {
        currentMonth = 1;
        currentYear++;
        char strYear[10];
        sprintf(strYear, "%d", currentYear);
        gtk_label_set_text(GTK_LABEL(yearLbl), strYear);
        gtk_label_set_text(GTK_LABEL(yearLblSmall), strYear);

    } else {
        currentMonth++;
    }
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char strd[] = "", strNumD[10];
    strcat(strd, (char*)months[currentMonth - 1]);
    strcat(strd, strNumD);
    sprintf(strNumD, "%d", tm.tm_mday);
    strcat(strd, ", ");
    strcat(strd, strNumD);
    gtk_label_set_text(GTK_LABEL(lbCurrentDate), strd);
    gtk_label_set_text(GTK_LABEL(monthLblSmall), (char*)months[currentMonth - 1]);
    makeCalendar();
}
G_MODULE_EXPORT void on_btnToDay_clicked(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    currentYear = tm.tm_year + 1900;
    char strYear[4];
    sprintf(strYear, "%d", currentYear);
    currentMonth = tm.tm_mon + 1;
    gtk_label_set_text(GTK_LABEL(yearLbl), strYear);
    gtk_label_set_text(GTK_LABEL(yearLblSmall), strYear);
    gtk_label_set_text(GTK_LABEL(monthLblSmall), (char*)months[tm.tm_mon]);


    char strd[] = "", strNumD[5];
    strcat(strd, (char*)months[tm.tm_mon]);
    strcat(strd, strNumD);
    sprintf(strNumD, "%d", tm.tm_mday);
    strcat(strd, ", ");
    strcat(strd, strNumD);
    gtk_label_set_text(GTK_LABEL(lbCurrentDate), strd);

    makeCalendar();
}
//dark mode
G_MODULE_EXPORT void on_darkModeBtn_state_set(int argc, char *argv[]) {
    if (darkModeState == 1) darkModeState = 0;
    else darkModeState = 1;
    gtk_widget_destroy(GTK_WIDGET(windowCalendar));
    showCalendarWindow(argc, argv);
}
//EVENT WINDOW
G_MODULE_EXPORT void popdown() {
    gtk_popover_popdown(GTK_POPOVER(reviewEvent));
}
G_MODULE_EXPORT void on_windowImportant_toggled() {
    if (windowImportant == 1) windowImportant = 0;
    else windowImportant = 1;
    makeCalendar();
}
G_MODULE_EXPORT void on_windowWork_toggled() {
    if (windowWork == 1) windowWork = 0;
    else windowWork = 1;
    makeCalendar();
}
G_MODULE_EXPORT void on_windowReminder_toggled() {
    if (windowReminder == 1) windowReminder = 0;
    else windowReminder = 1;
    makeCalendar();
}

G_MODULE_EXPORT void on_popupCalendar_day_selected() {
    gtk_entry_set_text(GTK_ENTRY(dateEventInput), "");
    int selectDay, selectMonth, selectYear;
    gtk_calendar_get_date(GTK_CALENDAR(popupCalendar), &selectYear, &selectMonth, &selectDay);
    char dateStr[12], buf[5];
    sprintf(buf, "%d", selectDay);
    strcat(dateStr, buf);
    strcat(dateStr, "/");
    sprintf(buf, "%d", selectMonth + 1);
    strcat(dateStr, buf);
    strcat(dateStr, "/");
    sprintf(buf, "%d", selectYear);
    strcat(dateStr, buf);

    for (int i = 0; i < strlen(dateStr); i++) {
        if (dateStr[i] < '\n' || dateStr[i] > '~') {
            dateStr[i] = ' ';
        }
    }
    gtk_entry_set_text(GTK_ENTRY(dateEventInput), dateStr);
}
G_MODULE_EXPORT void on_cancelBtn_clicked() {
    gtk_popover_popdown(GTK_POPOVER(popover3));
}
G_MODULE_EXPORT void on_createBtn_clicked() {
    int selectDay, selectMonth, selectYear;
    gtk_calendar_get_date(GTK_CALENDAR(popupCalendar), &selectYear, &selectMonth, &selectDay);
    const char* eventTitle = gtk_entry_get_text(GTK_ENTRY(evenTitleInput));

    GtkTextIter start, end;
    createEventBuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(eventContentInput));
    gtk_text_buffer_get_bounds(createEventBuf, &start, &end);
    char *eventContent = gtk_text_buffer_get_text(createEventBuf, &start, &end, FALSE);
    for (int i = 0; i < strlen(eventContent); i++) {
        if (eventContent[i] == '\n') {
            eventContent[i] = '~';
        }
    }
    if (strlen(eventContent) != 0 && strlen(eventTitle) != 0) {
            int eventFeature = 0;
        if (reminderCheck == 1) {
            eventFeature = 1;
        } else if (workCheck == 1) {
            eventFeature = 2;
        } else if (importantCheck == 1) {
            eventFeature = 3;
        }
        //reminder - work - important
        setEvent(selectDay, selectMonth + 1, selectYear, eventTitle, eventContent, eventFeature, 3);
        gtk_text_buffer_set_text(createEventBuf, "", 0);
        gtk_text_view_set_buffer(GTK_TEXT_VIEW(eventContentInput), createEventBuf);
        gtk_entry_set_text(GTK_ENTRY(evenTitleInput), "");
        gtk_popover_popdown(GTK_POPOVER(popover3));
        makeCalendar();

    }

}
G_MODULE_EXPORT void on_importantCheck_toggled() {
    if (importantCheck == 0) importantCheck = 1;
    else importantCheck = 0;
}
G_MODULE_EXPORT void on_reminderCheck_toggled() {
    if (reminderCheck == 0) reminderCheck = 1;
    else reminderCheck = 0;
}
G_MODULE_EXPORT void on_workCheck_toggled() {
    if (workCheck == 0) workCheck = 1;
    else workCheck = 0;
}

//EVENT REVIEW WINDOW
G_MODULE_EXPORT void on_downImportant_clicked() {
    curImportant--;
    showEventReview(curReviewWindow);
}
G_MODULE_EXPORT void on_upImportant_clicked() {
    curImportant++;
    showEventReview(curReviewWindow);
}
G_MODULE_EXPORT void on_clearImportant_clicked() {
    if (curImportantID > 0) {
        clearEvent(curReviewWindow, currentMonth, currentYear, curImportantID, 3);
        curImportant--;
        showEventReview(curReviewWindow);
        makeCalendar();
    }
}
G_MODULE_EXPORT void on_downReminder_clicked() {
    curReminder--;
    showEventReview(curReviewWindow);
}
G_MODULE_EXPORT void on_upReminder_clicked() {
    curReminder++;
    showEventReview(curReviewWindow);
}
G_MODULE_EXPORT void on_clearReminder_clicked() {
    if (curReminderID > 0) {
        clearEvent(curReviewWindow, currentMonth, currentYear, curReminderID, 3);
        curReminder--;
        showEventReview(curReviewWindow);
        makeCalendar();
    }
}
G_MODULE_EXPORT void on_downWork_clicked() {
    curWork--;
    showEventReview(curReviewWindow);
}
G_MODULE_EXPORT void on_upWork_clicked() {
    curWork++;
    showEventReview(curReviewWindow);
}
G_MODULE_EXPORT void on_clearWork_clicked() {
    if (curWorkID > 0) {
        clearEvent(curReviewWindow, currentMonth, currentYear, curWorkID, 3);
        curWork--;
        showEventReview(curReviewWindow);
        makeCalendar();
    }
}


G_MODULE_EXPORT void on_dateBtn1_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[1]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn2_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[2]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn3_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[3]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn4_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[4]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn5_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[5]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn6_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[6]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn7_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[7]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn8_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[8]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn9_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[9]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn10_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[10]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn11_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[11]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn12_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[12]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn13_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[13]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn14_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[14]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn15_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[15]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn16_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[16]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn17_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[17]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn18_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[18]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn19_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[19]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn20_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[20]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn21_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[21]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn22_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[22]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn23_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[23]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn24_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[24]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn25_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[25]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn26_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[26]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn27_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[27]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn28_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[28]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn29_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[29]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn30_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[30]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn31_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[31]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn32_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[32]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn33_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[33]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn34_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[34]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}
G_MODULE_EXPORT void on_dateBtn35_clicked() {
    const char* strEventDay = gtk_label_get_text(GTK_LABEL(solarDay[35]));
    //check empty cell
    if (strlen(strEventDay)) {
        showEventReview(atoi(strEventDay));
    } else showEventReview(0);
    curImportant = 1;
    curReminder = 1;
    curWork = 1;
}


// LOGIN WINDOW
// call to change to Sign in window
G_MODULE_EXPORT void onClicked_sign_change(int argc, char *argv[])
{
    gtk_widget_destroy(GTK_WIDGET(windowLogin));
    showSignupWindow(argc, argv);
}

// called when click login
G_MODULE_EXPORT void onClicked_Login(int argc, char *argv[])
{
    // variable to save GTK Entry
    char userTxtLogin[128];
    char passTxtLogin[128];

    // put text from entry to variable
    sprintf(userTxtLogin, "%s", gtk_entry_get_text(userEntryLogin));
    sprintf(passTxtLogin, "%s", gtk_entry_get_text(passEntryLogin));

    bool isValidUsernameTxt = isValidUsername(userTxtLogin);
    bool isValidPasswordTxt = isValidPassword(passTxtLogin);

    if (!isValidUsernameTxt)
    {
        //Do something if username is invalid
        gtk_label_set_text(GTK_LABEL(lbUserCorLogin), "*[a-z], [A-Z], number, size < 20");

    }
    else if (!isValidPasswordTxt)
    {
        //Do something if password is invalid
        gtk_label_set_text(GTK_LABEL(lbPassCorLogin), "*[a-z], [A-Z], number, size < 20");
    }
    else {
        int status = userLogin(userTxtLogin, passTxtLogin);

        if (status == -1) {
            gtk_label_set_text(GTK_LABEL(lbUserCorLogin), "*have not account");
        } else if (status == 0) {
            //Do something if there is such account but wrong password
            gtk_label_set_text(GTK_LABEL(lbPassCorLogin), "*wrong password");
        } else {
            //Do something if there is such account and right password
            getUserByUsername(userTxtLogin, &user);
            saveIDPointer = fopen(SAVE_ID, "w");
            if (saveIDPointer != NULL) {
                    
                fprintf(saveIDPointer, "%d\n", user.id);
            }
            fclose(saveIDPointer);
            sprintf(currUserName, "%s", userTxtLogin);
            gtk_widget_destroy(GTK_WIDGET(windowLogin));
            gtk_widget_destroy(GTK_WIDGET(windowCalendar));
            showCalendarWindow(argc, argv);

        }
    }
}
G_MODULE_EXPORT void on_userEntryLogin_changed()
{
    gtk_label_set_text(GTK_LABEL(lbUserCorLogin), "");
}
G_MODULE_EXPORT void on_passEntryLogin_changed()
{
    gtk_label_set_text(GTK_LABEL(lbPassCorLogin), "");
}

G_MODULE_EXPORT void on_btnViewPassLogin_pressed()
{
    gtk_entry_set_visibility(passEntryLogin, TRUE);
}
G_MODULE_EXPORT void on_btnViewPassLogin_released()
{
    gtk_entry_set_visibility(passEntryLogin, FALSE);
}
// called when window is closed
G_MODULE_EXPORT void on_window_login_destroy()
{
    gtk_main_quit();
}

// SIGNUP WINDOW
// call to change to Log in window
G_MODULE_EXPORT void onClicked_login_change(int argc, char *argv[])
{
    gtk_widget_destroy(GTK_WIDGET(windowSign));
    showLoginWindow(argc, argv);
}

// called when click sign up
G_MODULE_EXPORT void onClicked_Signup()
{
    // variable to save GTK Entry
    char nameTxtSign[128];
    char userTxtSign[128];
    char passTxtSign[128];
    char confPassTxtSign[128];

    // put text from entry to variable
    sprintf(nameTxtSign, "%s", gtk_entry_get_text(nameEntrySign));
    sprintf(userTxtSign, "%s", gtk_entry_get_text(userEntrySign));
    sprintf(passTxtSign, "%s", gtk_entry_get_text(passEntrySign));
    sprintf(confPassTxtSign, "%s", gtk_entry_get_text(confPassEntrySign));

    //Check validation
    bool isValidNameTxt = isValidName(nameTxtSign);
    bool isValidUsernameTxt = isValidUsername(userTxtSign);
    bool isValidPasswordTxt = isValidPassword(passTxtSign);
    bool isUserNameExist = isUserUsernameExisted(userTxtSign);
    if (!isValidNameTxt)
    {
        //Do something if name is wrong
        gtk_label_set_text(GTK_LABEL(lbNameCor), "*[a-z], [A-Z], size < 40");
    }
    else if (!isValidUsernameTxt)
    {
        //Do something if username is wrong
        gtk_label_set_text(GTK_LABEL(lbUserCor), "*[a-z], [A-Z], number, size < 20");
    }
    else if (isUserNameExist)
    {
        //Do something if username is wrong
        gtk_label_set_text(GTK_LABEL(lbUserCor), "*This username is already existed");
    }
    else if (!isValidPasswordTxt)
    {
        //Do something if password is wrong
        gtk_label_set_text(GTK_LABEL(lbPassCor), "*[a-z], [A-Z], number, size < 20");
    }
    else {
        if (strcmp(passTxtSign, confPassTxtSign) != 0)
        {
            //Do something if password doesn't match
            gtk_label_set_text(GTK_LABEL(lbConfCor), "*not same with confirm password");
        }
        else {
            user.name = nameTxtSign;
            user.username = userTxtSign;
            user.password = passTxtSign;

            if (registerNewAccount(user) == 1){
                //Do something if register successfully
                gtk_widget_destroy(GTK_WIDGET(windowSign));
                gtk_widget_show(dialogSuccess);
            }
        }
    }




}
G_MODULE_EXPORT void on_nameEntry_changed(){
    gtk_label_set_text(GTK_LABEL(lbNameCor), "");
}
G_MODULE_EXPORT void on_userEntry_changed(){
    gtk_label_set_text(GTK_LABEL(lbUserCor), "");
}
G_MODULE_EXPORT void on_passEntry_changed(){
    gtk_label_set_text(GTK_LABEL(lbPassCor), "");
}
G_MODULE_EXPORT void on_confPassEntry_changed(){
    gtk_label_set_text(GTK_LABEL(lbConfCor), "");
}

G_MODULE_EXPORT void on_btnViewPassSignup_pressed()
{
    gtk_entry_set_visibility(passEntrySign, TRUE);
}
G_MODULE_EXPORT void on_btnViewPassSignup_released()
{
    gtk_entry_set_visibility(passEntrySign, FALSE);
}

G_MODULE_EXPORT void on_btnViewConfSignup_pressed()
{
    gtk_entry_set_visibility(confPassEntrySign, TRUE);
}
G_MODULE_EXPORT void on_btnViewConfSignup_released()
{
    gtk_entry_set_visibility(confPassEntrySign, FALSE);
}

// called when window is closed
G_MODULE_EXPORT void on_window_sign_destroy()
{
    gtk_main_quit();
}
// ADMIN WINDOW
// called for admin window
G_MODULE_EXPORT void onClicked_Admin(int argc, char *argv[])
{
    showLoginAdminWindow(argc, argv);
}
G_MODULE_EXPORT void on_window_admin_destroy()
{
	gtk_main_quit();
}
G_MODULE_EXPORT void onClicked_Login_Admin(int argc, char *argv[]){
	// variable to save GTK Entry
    char userTxtAdmin[128];
    char passTxtAdmin[128];

    // put text from entry to variable
    sprintf(userTxtAdmin, "%s", gtk_entry_get_text(userEntryAdmin));
    sprintf(passTxtAdmin, "%s", gtk_entry_get_text(passEntryAdmin));

    bool isValidUsernameTxt = isValidUsername(userTxtAdmin);
    bool isValidPasswordTxt = isValidPassword(passTxtAdmin);

    if (!isValidUsernameTxt)
    {
        //Do something if username is invalid
        gtk_label_set_text(GTK_LABEL(lbUserCorAdmin), "*[a-z], [A-Z], number, size < 20");

    }
    else if (!isValidPasswordTxt)
    {
        //Do something if password is invalid
        gtk_label_set_text(GTK_LABEL(lbPassCorAdmin), "*[a-z], [A-Z], number, size < 20");
    }
    else {
        int status = adminLogin(userTxtAdmin, passTxtAdmin);

        if (status == -1) {
            gtk_label_set_text(GTK_LABEL(lbUserCorAdmin), "*have not account");
        } else if (status == 0) {
            //Do something if there is such account but wrong password
            gtk_label_set_text(GTK_LABEL(lbPassCorAdmin), "*wrong password");
        } else {
            //Do something if there is such account and right password
            gtk_widget_destroy(GTK_WIDGET(windowLoginAdmin));
            showManageUserWindow(argc, argv);
        }
    }

}

G_MODULE_EXPORT void on_userEntryAdmin_changed(){
	gtk_label_set_text(GTK_LABEL(lbUserCorAdmin), "");
}
G_MODULE_EXPORT void on_passwordEntryAdmin_changed(){
	gtk_label_set_text(GTK_LABEL(lbUserCorAdmin), "");
}

G_MODULE_EXPORT void on_btnViewPassAdmin_pressed()
{
    gtk_entry_set_visibility(passEntryAdmin, TRUE);
}
G_MODULE_EXPORT void on_btnViewPassAdmin_released()
{
    gtk_entry_set_visibility(passEntryAdmin, FALSE);
}
//Manage window
// when selected 
G_MODULE_EXPORT void on_selection_changed()
{
    gchar *value;
    GtkTreeIter iter;
    GtkTreeModel *model;

    if (gtk_tree_selection_get_selected(selection, &model, &iter) == FALSE)
        return;
    gtk_tree_model_get(model, &iter, 1, &value, -1);
    sprintf(manageUsername, "%s", value);
}
// show update window
G_MODULE_EXPORT void on_btnUpdate_clicked(int argc, char *argv[])
{
    if(user.id != 0)
    {
        showUpdateWindow(argc, argv);
    }
}
G_MODULE_EXPORT void on_btnGetUpdate_clicked(int argc, char *argv[])
{
    // variable to save GTK Entry
    char userTxtUpdate[128];
    char passTxtUpdate[128];
    char confPassTxtUpdate[128];
    
    // put text from entry to variable
    sprintf(userTxtUpdate, "%s", gtk_entry_get_text(userEntryUpdate));
    sprintf(passTxtUpdate, "%s", gtk_entry_get_text(passEntryUpdate));
    sprintf(confPassTxtUpdate, "%s", gtk_entry_get_text(confPassEntryUpdate));
    
    //Check validation
    bool isValidUsernameTxt = isValidUsername(userTxtUpdate);
    bool isValidPasswordTxt = isValidPassword(passTxtUpdate);
    bool isUserNameExist = isUserUsernameExisted(userTxtUpdate);

    User userUpdate = {};
    getUserByID(user.id, &userUpdate);

    if (!isValidUsernameTxt)
    {
        //Do something if username is wrong
        gtk_label_set_text(GTK_LABEL(lbUserCorUpdate), "*[a-z], [A-Z], number, size < 20");
    }
    else if (isUserNameExist == true && strcmp(userTxtUpdate, userUpdate.username) != 0)
    {
        //Do something if username is wrong
        gtk_label_set_text(GTK_LABEL(lbUserCorUpdate), "*This username is already existed");
    }    
    else if (!isValidPasswordTxt)
    {
        //Do something if password is wrong
        gtk_label_set_text(GTK_LABEL(lbPassCorUpdate), "*[a-z], [A-Z], number, size < 20");
    }    
    else {
        if (strcmp(passTxtUpdate, confPassTxtUpdate) != 0)
        {
            //Do something if password doesn't match
            gtk_label_set_text(GTK_LABEL(lbConfCorUpdate), "*not same with confirm password");
        }   
        else {
            updateUsername(userUpdate.id, userTxtUpdate);
            updatePassword(userUpdate.id, passTxtUpdate);
            gtk_widget_destroy(GTK_WIDGET(windowUpdate));
            gtk_widget_show(dialogSuccessUpdate);
            
        }
    }
}
G_MODULE_EXPORT void on_userEntryUpdate_changed()
{
    gtk_label_set_text(GTK_LABEL(lbUserCorUpdate), "");
}
G_MODULE_EXPORT void on_passEntryUpdate_changed()
{
    gtk_label_set_text(GTK_LABEL(lbPassCorUpdate), "");
}
G_MODULE_EXPORT void on_confPassEntryUpdate_changed()
{
    gtk_label_set_text(GTK_LABEL(lbConfCorUpdate), "");
}
G_MODULE_EXPORT void on_window_update_destroy()
{
    gtk_main_quit();
}
G_MODULE_EXPORT void on_btnViewPassUpdate_pressed()
{
    gtk_entry_set_visibility(passEntryUpdate, TRUE);
}
G_MODULE_EXPORT void on_btnViewPassUpdate_released()
{
    gtk_entry_set_visibility(passEntryUpdate, FALSE);
}

G_MODULE_EXPORT void on_btnViewConfUpdate_pressed()
{
    gtk_entry_set_visibility(confPassEntryUpdate, TRUE);
}
G_MODULE_EXPORT void on_btnViewConfUpdate_released()
{
    gtk_entry_set_visibility(confPassEntryUpdate, FALSE);
}
// delete user
G_MODULE_EXPORT void on_btnDelete_clicked()
{
    User userDelete = {};
    User userGet = {};
    GtkTreeIter iter;
    
    char txtID[128];
    char txtUsername[128];
    char txtName[128];
    char txtPass[128];
    getUserByUsername(manageUsername, &userDelete);
    deleteUser(&userDelete);
    int numUserID = getLastUserID();
    gtk_list_store_clear(liststoreManage);
    for(int i = 1; i <= numUserID; i++)
    {
        getUserByID(i, &userGet);
        sprintf(txtID, "%d", userGet.id);
        sprintf(txtUsername, "%s", userGet.username);
        sprintf(txtName, "%s", userGet.name);
        sprintf(txtPass, "%s", userGet.password);
        gtk_list_store_append(liststoreManage, &iter);
        gtk_list_store_set (liststoreManage, &iter, 0, txtID, -1);
        gtk_list_store_set (liststoreManage, &iter, 1, txtUsername, -1);
        gtk_list_store_set (liststoreManage, &iter, 2, txtName, -1);
        gtk_list_store_set (liststoreManage, &iter, 3, txtPass, -1);
    }

}
// Sign out event
G_MODULE_EXPORT void on_btnSignout_clicked(int argc, char *argv[])
{
    user.id = 0;
    sprintf(currUserName, "%s", "Username");
    saveIDPointer = fopen(SAVE_ID, "w");
    fprintf(saveIDPointer, "%d\n", user.id);
    fclose(saveIDPointer);
    gtk_widget_destroy(windowCalendar);
    showCalendarWindow(argc, argv);
}
// call for successfull dialog
G_MODULE_EXPORT void on_btnOk_clicked(int argc, char *argv[]){
    gtk_widget_destroy(GTK_WIDGET(dialogSuccess));
    showLoginWindow(argc, argv);
}
G_MODULE_EXPORT void on_btnOkUpdate_clicked(int argc, char *argv[]){
    gtk_widget_destroy(GTK_WIDGET(dialogSuccessUpdate));
    showLoginWindow(argc, argv);
}
