#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../header/Login.h"
#include "../header/LunarCalendar.h"
//#include "../header/Layout.h"
//#include "calendarLayout.c"
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
void showLoginWindow(int argc, char *argv[])
{
	GtkBuilder *builderLogin;

    gtk_init(&argc, &argv);

    builderLogin = gtk_builder_new();
    gtk_builder_add_from_file (builderLogin, "calendarLogin.glade", NULL);

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
    gtk_builder_add_from_file (builderSign, "calendarSign.glade", NULL);
    
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
    gtk_builder_add_from_file (builderLoginAdmin, "calendarLoginAdmin.glade", NULL);

    windowLoginAdmin = GTK_WIDGET(gtk_builder_get_object(builderLoginAdmin, "window_login_Admin"));
    gtk_builder_connect_signals(builderLoginAdmin, NULL);

    g_object_unref(builderLoginAdmin);

    gtk_widget_show(windowLoginAdmin);
    gtk_main();
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
            gtk_widget_destroy(GTK_WIDGET(windowLogin));
            //gtk_widget_destroy(GTK_WIDGET(windowCalendar));
            //showCalendarWindow(argc, argv);
            
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

// called for admin window
G_MODULE_EXPORT void onClicked_Admin(int argc, char *argv[])
{
	showLoginAdminWindow(argc, argv);
}

G_MODULE_EXPORT void on_btnOk_clicked(int argc, char *argv[]){
	gtk_widget_destroy(GTK_WIDGET(dialogSuccess));
	showLoginWindow(argc, argv);
}
