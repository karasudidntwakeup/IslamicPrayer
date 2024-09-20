#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PRAYERS 6
#define TIME_STR_LEN 9

typedef struct {
    char name[20];
    char time[TIME_STR_LEN]; // HH:MM:SS
} Prayer;

void parse_time(const char *time_str, int *hours, int *minutes, int *seconds) {
    sscanf(time_str, "%d:%d:%d", hours, minutes, seconds);
}

int main() {
    Prayer prayers[MAX_PRAYERS];
    int prayer_count = 0;

    // Open the prayer times file
    FILE *file = fopen("prayer_times.txt", "r");
    if (!file) {
        perror("Unable to open prayer_times.txt");
        return 1;
    }

    // Read prayer times from the file
    while (fscanf(file, "%19s %8s", prayers[prayer_count].name, prayers[prayer_count].time) == 2) {
        prayer_count++;
        if (prayer_count >= MAX_PRAYERS) {
            break; // Avoid exceeding array bounds
        }
    }
    fclose(file);

    // Get current time
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    int current_seconds = local_time->tm_hour * 3600 + local_time->tm_min * 60 + local_time->tm_sec;

    // Find the next prayer
    for (int i = 0; i < prayer_count; i++) {
        int prayer_hours, prayer_minutes, prayer_seconds;
        parse_time(prayers[i].time, &prayer_hours, &prayer_minutes, &prayer_seconds);
        int prayer_seconds_total = prayer_hours * 3600 + prayer_minutes * 60 + prayer_seconds;

        if (prayer_seconds_total > current_seconds) {
            int remaining_seconds = prayer_seconds_total - current_seconds;
            int hours = remaining_seconds / 3600;
            int minutes = (remaining_seconds % 3600) / 60;
            int seconds = remaining_seconds % 60;

            // Output the result
            printf("Next prayer: %s\n", prayers[i].name);
            printf("Remaining time: %02d:%02d:%02d\n", hours, minutes, seconds);
            return 0; // Exit after finding the next prayer
        }
    }

    // If no next prayer found, assume it's the last prayer
    printf("No upcoming prayers found.\n");
    return 0;
}
