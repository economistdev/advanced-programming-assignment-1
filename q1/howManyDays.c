
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

struct Date {
	int day;
	int month;
	int year;
};

bool is_leap_year(int year) {
	if (year % 4 != 0) return false;
	if (year % 100 == 0 && year % 400 != 0) return false;

	return true;
}

bool cursor_leq_end_date(struct Date d1, struct Date d2) {
	bool cond_yr = d1.year < d2.year;
	bool cond_mo = d1.year == d2.year && d1.month < d2.month;
	bool cond_dy = d1.year == d2.year && d1.month == d2.month && d1.day <= d2.day;
	return (cond_yr || cond_mo || cond_dy);
}

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

int* days_between_dates(struct Date start_date, struct Date end_date, int *days_histogram, bool twelth_of_month) {
	struct Date date_cursor = start_date;
	int day_of_week = 0;

	// days historgram integers are in no particular order, i.e. day 1 is not necessarily monday
	// integers/index will prescribe to a new mapping to days of week each time, depending on start date
	//zero histogram out
	for (int i = 0; i < 7; i++) {
		days_histogram[i] = 0;
	}


	while (cursor_leq_end_date(date_cursor, end_date)) {
		int days_in_current_month = days_in_month(date_cursor.month, date_cursor.year);
		//printf("%d days in curr month\n", days_in_current_month);
		//printf("%d/%d/%d\n", date_cursor.day, date_cursor.month, date_cursor.year);
		//printf("day of week: %d\n", day_of_week);

		if ((twelth_of_month && date_cursor.day == 12) || (days_histogram[day_of_week] += 1)) {
			days_histogram[day_of_week] += 1;
		}

		day_of_week = (day_of_week + 1) % 7;

		if (date_cursor.day == days_in_current_month) {
			if (date_cursor.month == 12) {
				date_cursor.day = 1;
				date_cursor.month = 1;
				date_cursor.year++;
			} else {
				date_cursor.day = 1;
				date_cursor.month++;
			}
		} else {
			date_cursor.day++;
		}
	}

	return days_histogram;

}

int sum_days_histogram(const int *days_histogram) {
	int accumulator = 0;
	for (int i = 0; i < 7; i++) {
		accumulator += days_histogram[i];
	}
	return accumulator;
}

int howManyDays(void) {

	// Dates between 1401-01-01 and 1800-12-31 as "main" interval to check
	struct Date date_start_main = (struct Date) {1, 1, 1401};
	struct Date date_end_main = (struct Date) {31, 12, 1800};

	// Get the historgram of days between dates (no mapping to index and actual day of week yet)
	int days_histogram_main[7];
	int days_histogram_main_on_twelth[7];
	days_between_dates(date_start_main, date_end_main, days_histogram_main, false);
	days_between_dates(date_start_main, date_end_main, days_histogram_main_on_twelth, true); // same as above but only for
	//days falling on twelth of month

	// Sum the histogram
	int sum_days_main = sum_days_histogram(days_histogram_main);

	// Date to tuesday! We need this to map the dates histogram back to an actual day
	struct Date date_start_to_tuesday = (struct Date) {1, 1, 1801};
	struct Date date_end_to_tuesday = (struct Date) {18, 05, 2007};
	// Get the historgram of days between dates again
	int days_histogram_to_tuesday[7];
	days_between_dates(date_start_to_tuesday, date_end_to_tuesday, days_histogram_to_tuesday, false);
	// Sum the histogram
	int sum_days_to_tuesday = sum_days_histogram(days_histogram_to_tuesday);

	// gets us to the index for monday
	int hist_selected_index = (sum_days_to_tuesday + sum_days_main + 6) % 7; // +6 is to simulate tuesday to monday

	return days_histogram_main_on_twelth[hist_selected_index];
}

void test() {

	//test 1.1: check number "days_between_dates" function
	struct Date date_start = (struct Date) {25, 12, 2024};
	struct Date date_end = (struct Date) {5, 1, 2025};
	int days_histogram[7];
	days_between_dates(date_start, date_end, days_histogram, false);
	int sum_days = sum_days_histogram(days_histogram);
	printf("test 1.1: %d\n", sum_days);

}

int main() {

	printf("%d\n", howManyDays());
	//test();

	return 0;
}



