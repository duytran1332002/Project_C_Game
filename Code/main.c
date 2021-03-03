#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../header/solarLogic.c"
#include <string.h>
#include "../header/UserLogin.h"
#include "../header/AdminLogin.h"
#include "../header/LunarCalendar.h"

int currentMonth;
int currentYear;
char currUserName[128];
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
// Manage window
GtkWidget *windowManage;
// successfull dialog
GtkWidget *dialogSuccess;
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
//=============Load css function=================
static void load_css(void)
{
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;
    ///***
    const gchar *css_style_file = "../gui/style.css";
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
    }
    LunarDate lnDate;
    for (int i = numEmptyDay + 1; i <= 35; i++)
    {
        if (numDay <= maxDay[currentMonth - 1])
        {
            gtk_label_set_text(GTK_LABEL(solarDay[i]), date[numDay]);
            gtk_label_set_text(GTK_LABEL(solarSmallDay[i]), date[numDay]);
            lnDate = convertSolarToLunar(numDay, currentMonth, currentYear, 7.0);
            char str[] = "", strNum[10];
            sprintf(strNum, "%d", lnDate.day);
            strcat(str, strNum);
            sprintf(strNum, "%d ", lnDate.month);
            strcat(str, "/");
            strcat(str, strNum);
            gtk_label_set_text(GTK_LABEL(lunarDay[i]), str);
            numDay++;
        }
    }
    while (numDay <= maxDay[currentMonth - 1])
    {
        gtk_label_set_text(GTK_LABEL(solarDay[postDay]), date[numDay]);
        gtk_label_set_text(GTK_LABEL(solarSmallDay[postDay]), date[numDay]);
        lnDate = convertSolarToLunar(numDay, currentMonth, currentYear, 7.0);
        char str[] = "", strNum[10];
        sprintf(strNum, "%d", lnDate.day);
        strcat(str, strNum);
        sprintf(strNum, "%d", lnDate.month);
        strcat(str, "/");
        strcat(str, strNum);
        gtk_label_set_text(GTK_LABEL(lunarDay[postDay]), str);
        numDay++;
        postDay++;
    }
}
void showCalendarWindow(int argc, char *argv[])
{
    GtkBuilder *builderCalendar;
    
    gtk_init(&argc, &argv);
    builderCalendar = gtk_builder_new();
    gtk_builder_add_from_file(builderCalendar, "../gui/CalendarlayoutYes.glade", NULL);

    windowCalendar = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "window_main"));
    yearLbl = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "year"));
    yearLblSmall = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "yearSmall"));
    monthLblSmall = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "monthSmall"));
    lbCurrentDate = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "lbCurrentDate"));
    lbUserName = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "lbUserName"));
    
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
    load_css();
    gtk_main();
}
void showLoginWindow(int argc, char *argv[])
{
    GtkBuilder *builderLogin;

    gtk_init(&argc, &argv);

    builderLogin = gtk_builder_new();
    gtk_builder_add_from_file (builderLogin, "../gui/calendarLogin.glade", NULL);

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
    gtk_builder_add_from_file (builderSign, "../gui/calendarSign.glade", NULL);
    
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
    gtk_builder_add_from_file (builderLoginAdmin, "../gui/calendarLoginAdmin.glade", NULL);
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

    gtk_init(&argc, &argv);

    builderManage = gtk_builder_new();
    gtk_builder_add_from_file (builderManage, "../gui/CalendarManageUser.glade", NULL);

    windowManage = GTK_WIDGET(gtk_builder_get_object(builderManage, "window_manage"));
    liststoreManage = GTK_LIST_STORE(gtk_builder_get_object(builderManage, "liststoreManage"));
    treeViewManage = GTK_TREE_VIEW(gtk_builder_get_object(builderManage, "treeViewManage"));
    colID = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builderManage, "colID"));
    colUsername = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builderManage, "colUsername"));
    colName = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builderManage, "colName"));
    colPass = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builderManage, "colPass"));
    
    gtk_list_store_append(liststoreManage, &iter);
    gtk_list_store_set (liststoreManage, &iter, 0, "yes", -1);
    gtk_builder_connect_signals(builderManage, NULL);

    g_object_unref(builderManage);

    gtk_widget_show(windowManage);
    gtk_main();
}
int main(int argc, char *argv[])
{
    showCalendarWindow(argc, argv);
    return 0;
}

// destroy calendar window
G_MODULE_EXPORT void on_window_calendar_destroy()
{
    gtk_main_quit();
}
// destroy Manage window
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
            //User user = {};
            getUserByUsername(userTxtLogin, &user);
            sprintf(currUserName, "%s", userTxtLogin);
            g_print("%s", user.username);
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
        g_print("true");
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
        	//User user = {};
            user.name = nameTxtSign;
            user.username = userTxtSign;
            user.password = passTxtSign;
            
            if (registerNewAccount(user) == 1){
                //Do something if register successfully
                gtk_widget_destroy(GTK_WIDGET(windowSign));
                gtk_widget_show_all(dialogSuccess);
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

// call for successfull dialog
G_MODULE_EXPORT void on_btnOk_clicked(int argc, char *argv[]){
    gtk_widget_destroy(GTK_WIDGET(dialogSuccess));
    showLoginWindow(argc, argv);
}
