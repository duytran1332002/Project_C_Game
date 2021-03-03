#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
#include "solarLogic.c"
#include "calendar_login_signup.c"
#include "../header/Login.h"
#include "../header/LunarCalendar.h"
int currentMonth;
int currentYear;
GtkWidget *windowCalendar;
GtkWidget *grid;
GtkWidget *yearLbl;
GtkWidget *yearLblSmall;
//GtkWidget *monthLbl;
GtkWidget *monthLblSmall;
GtkWidget *lbCurrentDate;
GtkWidget *lbUserName;
GtkWidget *solarDay[37];
GtkWidget *solarSmallDay[37]; 
GtkWidget *lunarDay[37];
GtkWidget *weekLbl[10];

//=============Load css function=================
static void load_css(void)
{
	GtkCssProvider *provider;
	GdkDisplay *display;
	GdkScreen *screen;
	///***
	const gchar *css_style_file = "style.css";
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
            sprintf(strNum, "%d ", lnDate.day);
            strcat(str, strNum);
            sprintf(strNum, " %d  ", lnDate.month);
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
    gtk_builder_add_from_file(builderCalendar, "CalendarlayoutYes.glade", NULL);

    windowCalendar = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "window_main"));
    yearLbl = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "year"));
    yearLblSmall = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "yearSmall"));
    monthLblSmall = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "monthSmall"));
    lbCurrentDate = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "lbCurrentDate"));
    lbUserName = GTK_WIDGET(gtk_builder_get_object(builderCalendar, "lbUserName"));
    
    // set text username label
    
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

int main(int argc, char *argv[])
{
	showCalendarWindow(argc, argv);
    return 0;
}

G_MODULE_EXPORT void on_window_calendar_destroy()
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














