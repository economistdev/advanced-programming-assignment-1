
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

bool cursor_less_equal_to_end_date(struct Date date1, struct Date date2) {
	if( date1.year > date2.year) {
		return false;
	}
	if (date1.month > date2.month) {
		return false;
	}
	if (date1.day > date2.day) {
		return false;
	}
	return true;
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

int* days_between_dates(struct Date start_date, struct Date end_date) {
	struct Date date_cursor = start_date;
	int day_of_week = 0;

	// days historgram integers are in no particular order, i.e. day 1 is not necessarily monday
	// integers/index will prescribe to a new mapping to days of week each time, depending on start date
	static int days_histogram[7] = {0};


	while (cursor_less_equal_to_end_date(date_cursor, end_date)) {
		int days_in_current_month = days_in_month(date_cursor.month, date_cursor.year);

		if (date_cursor.day > days_in_current_month) {
			if (date_cursor.month == 12) {
				date_cursor = (struct Date) {1, 1, date_cursor.year + 1};
			} else {
				date_cursor = (struct Date) {1, date_cursor.month + 1, date_cursor.year};
			}
		}

		days_histogram[day_of_week] += 1;
		day_of_week = day_of_week % 7;
	}

	return days_histogram;

}

int sum_days_histogram(int *days_histogram) {
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
	int *days_histogram_main = days_between_dates(date_start_main, date_end_main);
	// Sum the histogram
	int sum_days_main = sum_days_histogram(days_histogram_main);

	// Date to tuesday! We need this to map the dates histogram back to an actual day
	struct Date date_start_to_tuesday = (struct Date) {1, 1, 1801};
	struct Date date_start_main_tuesday = (struct Date) {18, 05, 2007};
	// Get the historgram of days between dates again
	int *days_histogram_to_tuesday = days_between_dates(date_start_to_tuesday, date_end_to_tuesday);
	// Sum the histogram
	int sum_days_to_tuesday = sum_days_histogram(days_histogram_to_tuesday);

	// gets us to the index for monday
	int hist_selected_index = (sum_days_to_tuesday + sum_days_main + 6) % 7; // +6 is to simulate tuessday to monday

}
int main() {

	//printf("%d\n", is_leap_year(2028));

	return 0;
}



