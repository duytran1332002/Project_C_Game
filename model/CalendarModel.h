#ifndef CALENDAR_MODEL_H
#define CALENDAR_MODEL_H

typedef struct strLunarDate {
    int day, month, year;
    bool isLeapYear;
} LunarDate;

typedef struct strDate {
    int day, month, year;
    bool isLeapYear;
} Date;

/*
 * Calendar styles
 */
#define JULIAN 0
#define GREGORIAN 1

/*
 * Sexagenarian cycle constants (Can Chi)
 */
const char* EARTHLY_BRANCHES_IN_ENG[] = {"Zi", "Chou", "Yin", "Mao", "Chen", "Si", "Wu", "Wei", "Shen", "You", "Xu", "Hai"};
const char* HEAVENLY_STEMS_IN_ENG[] = {"Jia", "Yi", "Bing", "Ding", "Wu", "Ji", "Geng", "Xin", "Ren", "Gui"};
const char* EARTHLY_BRANCHES_IN_VIE[] = {"Tý", "Sửu", "Dần", "Mẹo", "Thìn", "Tỵ", "Ngọ", "Mùi", "Thân", "Dậu", "Tuất", "Hợi"};
const char* HEAVENLY_STEMS_IN_VIE[] = {"Giáp", "Ất", "Bính", "Đinh", "Mậu", "Kỷ", "Canh", "Tân", "Nhâm", "Quý"};

const char* DAY_OF_WEEK[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
const char* MONTH_OF_YEAR[] = {
        "January", "February", "March",
        "April", "May", "June",
        "July", "August", "September",
        "October", "November", "December"
};

/*
 * This day is when the Gregorian calendar was introduced and used to replaced the Julian calendar.
 * (5.10.1852)
 */
#define GREGORIAN_DAY 2299161

/*
 * Macro to convert from degree unit to radian unit.
 */
#define PI_TO_RADIAN(x) x * PI / 180
#define PI 3.1415925635

#endif