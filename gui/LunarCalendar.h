#ifndef LUNAR_CALENDAR_H
#define LUNAR_CALENDAR_H

#include <math.h>
#include <stdbool.h>
#include "../model/CalendarModel.h"

/*
 * Prototypes
 */
bool isSolarLeapYear(int year);
bool isLunarLeapYear(int year);
int dateToJulianDayNumber(int day, int month, int year);
Date julianDateNumberToDate(int julianDayNumber);
int INT(double d);
double newMoonDay(int k);
double sunLongitude(int julianDayNumber);
int getNewMoonDay(int k, double timeZone);
double getSunLongitude(int julianDayNumber, double timeZone);
int getLunarMonth11(int year, double timeZone);
int getLeapMonthOffset(int a11, double timeZone);
LunarDate convertSolarToLunar(int day, int month, int year, double timeZone);
Date convertLunarToSolar(int lunarDay, int lunarMonth, int lunarYear, 
                            bool lunarLeap, double timeZone);

/*
 * isSolarLeapYear:
 * Returns whether a year is a leap year.
 */
bool isSolarLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

/*
 * isLunarLeapYear:
 * Returns whether a lunar year is a leap year.
 */
bool isLunarLeapYear(int year) {
  return year % 19 == 0 || year % 19 == 3 ||
          year % 19 == 6 || year % 19 == 9 ||
          year % 19 == 11 || year % 19 == 14 ||
          year % 19 == 17;
}

/*
 * dateToJulianDayNumber:
 * Returns an integer which is the Julian Day number of the given date
 * since January 1 4713 BC (Julian Calendar)
 * or November 24 4714 BC (Gregorian Calendar).
 */
int dateToJulianDayNumber(int day, int month, int year) {
	int tempDay, tempMonth, tempYear;

    tempDay = (14 - month) / 12;
    tempYear = year + 4800 - tempDay;
    tempMonth = month + 12 * tempDay - 3;

    int julianDay = day + (153 * tempMonth + 2) / 5 + 365 * tempYear + tempYear / 4 - tempYear / 100 + tempYear / 400 - 32045;
    if (julianDay < 2299161)
        julianDay = day + (153 * tempMonth + 2) / 5 + 365 * tempYear + tempYear / 4 - 32083;

    return julianDay;
}

/*
 * julianDayNumberToDate:
 * Returns a Date-type variable containing the actual date of the given Julian Day number.
 */
Date julianDayNumberToDate(int julianDayNumber) {
    Date date;
    int b, c, d, e, m;

    if (julianDayNumber >= GREGORIAN_DAY) {
        b = 0;
        c = julianDayNumber + 32082;
    } else {
        int a = julianDayNumber + 32044;
        b = (4 * a + 3) / 146097;
        c = a - (b * 146097) / 4;
    }

    d = (4 * c + 3) / 1461;
    e = c - (1462 * d) / 4;
    m = (5 * e + 2) / 153;

    date.day = e - (153 * m + 2) / 5 + 1;
    date.month = m + 3 - 12 * (m / 10);
    date.year = b * 100 + d - 4800 + m / 10;
    date.isLeapYear = isSolarLeapYear(date.year);

    return date;
}

/*
 * INT:
 * Returns the decimal part of the given double.
 */
int INT(double d) {
    return (int) floor(d);
}

/*
 * newMoonDay:
 * Returns Julian Day number of the k-th New moon after or before the New moon of 1.1.1900 13:51 GMT.
 * Accuracy : 2 minutes
 * Algorithm from Astronomical Algorithms, by Jean Meeus, 1998
 */
double newMoonDay(int k) {
    double T = k / 1236.85;
    double TSquare = T * T, TCube = TSquare * T;
    double Jd1 = 2415020.75933 + 29.53058868 * k + 0.0001178 * TSquare - 0.000000155 * TCube;
    Jd1 = Jd1 + 0.00033 * sin(PI_TO_RADIAN(166.56 + 132.87 * T - 0.009173 * TSquare));    // Mean new moon
    double M = 359.2242 + 29.10535608 * k - 0.0000333 * TSquare - 0.00000347 * TCube;           // Sun's mean anomaly
    double Mpr = 306.0253 + 385.81691806 * k + 0.0107306 * TSquare + 0.00001236 * TCube;        // Moon's mean anomaly
    double F = 21.2964 + 390.67050646 * k - 0.0016528 * TSquare - 0.00000239 * TCube;           // Moon's argument of latitude
    double C1=(0.1734 - 0.000393*T) * sin(PI_TO_RADIAN(M)) + 0.0021 * sin(PI_TO_RADIAN(2 * M));

    C1 = C1 - 0.4068 * sin(PI_TO_RADIAN(Mpr)) + 0.0161 * sin(PI_TO_RADIAN(2*Mpr));
    C1 = C1 - 0.0004 * sin(PI_TO_RADIAN(3 * Mpr));
    C1 = C1 + 0.0104 * sin(PI_TO_RADIAN(2 * F)) - 0.0051 * sin(PI_TO_RADIAN(M + Mpr));
    C1 = C1 - 0.0074 * sin(PI_TO_RADIAN(M - Mpr)) + 0.0004 * sin(PI_TO_RADIAN(2 * F + M));
    C1 = C1 - 0.0004 * sin(PI_TO_RADIAN(2 * F - M)) - 0.0006 * sin(PI_TO_RADIAN(2 * F + Mpr));
    C1 = C1 + 0.0010 * sin(PI_TO_RADIAN(2 * F - Mpr)) + 0.0005 * sin(PI_TO_RADIAN(2 * Mpr + M));

    double delta;

    if (T < -11) {
        delta= 0.001 + 0.000839 * T + 0.0002261 * TSquare - 0.00000845 * TCube - 0.000000081 * T * TCube;
    } else {
        delta= -0.000278 + 0.000265 * T + 0.000262 * TSquare;
    }

    double julianDay = Jd1 + C1 - delta;
    return julianDay;
}

/*
 * sunLongitude:
 * Returns solar longitude in degree.
 * Algorithm from Astronomical Algorithms, by Jean Meeus, 1998
 */
double sunLongitude(int julianDayNumber) {
    double T = (julianDayNumber - 2451545.0 ) / 36525;              // Time in Julian centuries from 2000-01-01 12:00:00 GMT
    double TSquare = T * T;
    double M = 357.52910 + 35999.05030 * T - 0.0001559 * TSquare - 0.00000048 * T * TSquare;    // Mean anomaly, degree
    double L0 = 280.46645 + 36000.76983 * T + 0.0003032 * TSquare;                              // Mean longitude, degree
    double DL = (1.914600 - 0.004817 * T - 0.000014 * TSquare) * sin(PI_TO_RADIAN(M));
    DL = DL + (0.019993 - 0.000101*T) * sin(PI_TO_RADIAN(2 * M)) + 0.000290 * sin(PI_TO_RADIAN(3 * M));

    double L = L0 + DL;                     // True longitude, degree
    L = L - 360 * (INT(L / 360));        // Normalize to (0, 360)
    return L;
}

int getNewMoonDay(int k, double timeZone) {
    double julianDay = newMoonDay(k);
    return INT(julianDay + 0.5 + timeZone / 24);
}

double getSunLongitude(int julianDayNumber, double timeZone) {
    return sunLongitude(julianDayNumber - 0.5 - timeZone / 24);
}

int getLunarMonth11(int year, double timeZone) {
    double offset = dateToJulianDayNumber(31, 12, year) - 2415021.076998695;
    int k = INT(offset / 29.530588853);
    int nm = getNewMoonDay(k, timeZone);
    int sunLongitude = INT(getSunLongitude(nm, timeZone) / 30);
    if (sunLongitude >= 9) {
        nm = getNewMoonDay(k - 1, timeZone);
    }

    return nm;
}

int getLeapMonthOffset(int a11, double timeZone) {
    int k = INT(0.5 + (a11 - 2415021.076998695) / 29.530588853);
    int last;           // Month 11 contains point of sun longitude 3*PI/2 (December solstice)
    int i = 1;          // We start with the month following lunar month 11
    int arc = INT(getSunLongitude(getNewMoonDay(k + i, timeZone), timeZone) / 30);
    do {
        last = arc;
        i++;
        arc = INT(getSunLongitude(getNewMoonDay(k + i, timeZone), timeZone) / 30);
    } while (arc != last && i < 14);

    return i - 1;
}

/*
 * convertSolarToLunar:
 * Returns a LunarDate-type variable containing the given date's corresponding lunar date.
 */
LunarDate convertSolarToLunar(int day, int month, int year, double timeZone) {
    LunarDate lunar;
    int dayNumber = dateToJulianDayNumber(day, month, year);
    int k = INT((dayNumber - 2415021.076998695) / 29.530588853);
    int monthStart = getNewMoonDay(k+1, timeZone);

    if (monthStart > dayNumber) {
        monthStart = getNewMoonDay(k, timeZone);
    }

    int a11 = getLunarMonth11(year, timeZone);
    int b11 = a11;
    if (a11 >= monthStart) {
        lunar.year = year;
        a11 = getLunarMonth11(year - 1, timeZone);
    } else {
        lunar.year = year+1;
        b11 = getLunarMonth11(year + 1, timeZone);
    }
    lunar.day = dayNumber - monthStart + 1;

    int diff = INT((monthStart - a11) / 29);
    lunar.isLeapYear = false;
    lunar.month = diff + 11;
    if (b11 - a11 > 365) {
        int leapMonthDiff = getLeapMonthOffset(a11, timeZone);
        if (diff >= leapMonthDiff) {
            lunar.month = diff + 10;
            if (diff == leapMonthDiff) {
                lunar.isLeapYear = true;
            }
        }
    }
    if (lunar.month > 12) {
        lunar.month = lunar.month - 12;
    }
    if (lunar.month >= 11 && diff < 4) {
        lunar.year -= 1;
    }

    return lunar;
}

/*
 * convertLunarToSolar:
 * Return a Date-type variable containing the given lunar date's corresponding date.
 */
Date convertLunarToSolar(int lunarDay, int lunarMonth, int lunarYear, bool lunarLeap, double timeZone) {
    int a11, b11;
    if (lunarMonth < 11) {
        a11 = getLunarMonth11(lunarYear - 1, timeZone);
        b11 = getLunarMonth11(lunarYear, timeZone);
    } else {
        a11 = getLunarMonth11(lunarYear, timeZone);
        b11 = getLunarMonth11(lunarYear + 1, timeZone);
    }
    int k = INT(0.5 + (a11 - 2415021.076998695) / 29.530588853);
    int offset = lunarMonth - 11;
    if (offset < 0) {
        offset += 12;
    }
    if (b11 - a11 > 365) {
        int leapOff = getLeapMonthOffset(a11, timeZone);
        int leapMonth = leapOff - 2;
        if (leapMonth < 0) {
            leapMonth += 12;
        }
        if (lunarLeap != 0 && lunarMonth != leapMonth) {
            printf("Invalid input!\n");

            Date date;
            date.day = 0;
            date.month = 0;
            date.year = 0;
            date.isLeapYear = false;

            return date;
        } else if (lunarLeap != 0 || offset >= leapOff) {
            offset += 1;
        }
    }
    int monthStart = getNewMoonDay(k + offset, timeZone);
    return julianDayNumberToDate(monthStart + lunarDay - 1);
}

#endif