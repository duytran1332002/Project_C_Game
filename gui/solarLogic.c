char *months[] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};
char *date[] = {
    "",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "10",
    "11",
    "12",
    "13",
    "14",
    "15",
    "16",
    "17",
    "18",
    "19",
    "20",
    "21",
    "22",
    "23",
    "24",
    "25",
    "26",
    "27",
    "28",
    "29",
    "30",
    "31",
};

int emptyDay[12];
int maxDay[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int numOfWeek[12];

void getVal(int year)
{
	int daycode;

	if(year% 4 == 0 && year%100 != 0 || year%400 == 0) {
		maxDay[1] = 29;
	} else {
		maxDay[1] = 28;
	}

	int d1, d2, d3;

	d1 = (year - 1.)/ 4.0;
	d2 = (year - 1.)/ 100.;
	d3 = (year - 1.)/ 400.;
	daycode = (year + d1 - d2 + d3) % 7;

	for (int month = 1; month <= 12; month++ )
	{

		// Correct the position for the first date
		emptyDay[month - 1] = daycode - 1;
		if (emptyDay[month - 1] < 0)
			emptyDay[month - 1]+= 7;
		// Set position for next month
		daycode = ( daycode + maxDay[month - 1] ) % 7;
	}
    //number week in year
    numOfWeek[0] = 1;
    if (emptyDay[0] > 3)
        numOfWeek[0] = 0;
    for (int month = 2; month <= 12; month++) {
        numOfWeek[month - 1] = numOfWeek[month - 2] + (maxDay[month - 2] + emptyDay[month - 2]) / 7;
    }
}


