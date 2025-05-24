#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

void print_header() {
    printf("Content-Type: text/html\n\n");
}

void print_html_start() {
    printf("<html><head><title>Split Result</title></head><body style='font-family:Arial;background-color:#f5f0e6;color:#4b2e1e;padding:20px;'>");
    printf("<h1>Split Result</h1>");
}

void print_html_end() {
    printf("</body></html>");
}

void parse_data(char *input, char names[MAX][50], float values[MAX], int *count) {
    char *line = strtok(input, "\n");
    *count = 0;
    while (line != NULL && *count < MAX) {
        char *equal = strchr(line, '=');
        if (equal != NULL) {
            *equal = '\0';
            strcpy(names[*count], line);
            values[*count] = atof(equal + 1);
            (*count)++;
        }
        line = strtok(NULL, "\n");
    }
}

void handle_manual(float total, char *data) {
    char names[MAX][50];
    float percentages[MAX];
    int count = 0;

    parse_data(data, names, percentages, &count);

    printf("<h2>Manual Split</h2><ul>");
    for (int i = 0; i < count; i++) {
        float amount = (percentages[i] / 100.0f) * total;
        printf("<li>%s should pay ₹%.2f <input type='checkbox'> Paid</li>", names[i], amount);
    }
    printf("</ul>");
}

void handle_smart(float total, char *data) {
    char names[MAX][50];
    float incomes[MAX];
    int count = 0, i;
    float total_income = 0.0;

    parse_data(data, names, incomes, &count);
    for (i = 0; i < count; i++)
        total_income += incomes[i];

    printf("<h2>Smart Split</h2><ul>");
    for (i = 0; i < count; i++) {
        float share = (incomes[i] / total_income) * total;
        printf("<li>%s should pay ₹%.2f <input type='checkbox'> Paid</li>", names[i], share);
    }
    printf("</ul>");
}

int main() {
    char *len_str = getenv("CONTENT_LENGTH");
    int len = len_str ? atoi(len_str) : 0;
    char post_data[4096] = {0};
    char total_str[20] = {0};
    char mode[10] = {0};
    char manual_data[2048] = {0};
    char smart_data[2048] = {0};
    float total = 0.0;

    if (len > 0) {
        fread(post_data, 1, len, stdin);

        // Parse using strstr and sscanf
        sscanf(strstr(post_data, "total="), "total=%[^&]", total_str);
        sscanf(strstr(post_data, "mode="), "mode=%[^&]", mode);
        total = atof(total_str);

        char *manual_ptr = strstr(post_data, "manual_data=");
        char *smart_ptr = strstr(post_data, "smart_data=");

        if (manual_ptr)
            sscanf(manual_ptr, "manual_data=%[^&]", manual_data);
        if (smart_ptr)
            sscanf(smart_ptr, "smart_data=%[^&]", smart_data);

        // Decode + signs into spaces (HTML form encoding)
        for (int i = 0; manual_data[i]; i++)
            if (manual_data[i] == '+') manual_data[i] = ' ';
        for (int i = 0; smart_data[i]; i++)
            if (smart_data[i] == '+') smart_data[i] = ' ';

        // Convert %0A (newlines) if necessary — skip for now

        print_header();
        print_html_start();

        if (strcmp(mode, "manual") == 0)
            handle_manual(total, manual_data);
        else
            handle_smart(total, smart_data);

        print_html_end();
    }

    return 0;
}
