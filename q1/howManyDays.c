
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>


bool is_leap_year(int year) {
	if (year % 4 != 0 || year % 400 != 0) {
		return false;
	}
	return true;
}

bool date_is_greater_than(struct Date date1, struct Date date2) {
	if( date1.year > date2.year) {
		return true;
	}
	if (date1.month > date2.month) {
		return true;
	}
	if (date1.day > date2.day) {
		return true;
	}
	return false;
}

struct Date {
	int day;
	int month;
	int year;
};

int days_in_month(int month, int year) {
	if (month == 4 || month == 6 || month == 9 || month == 11) {
		return 30;
	} else if (month == 2) {
		if (is_leap_year(year)) {
			return 29;
		} else {
			return 28;
		}
	} else {
		return 31;
	}
}

int* days_between_dates(struct Date start_date, struct Date end_date) {
	struct Date date_cursor = start_date;
	int day_of_week = 0
	static int days_histogram[7] = {0}

	while (date_is_greater_than(date_cursor, end_date)) {

	}


}


int main() {

	//printf("%d\n", is_leap_year(2028));

	return 0;
}



