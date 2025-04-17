#include "scripts.h"

char* concatenateCString(const char* str1, const char* str2) {
    size_t len1 = std::strlen(str1);
    size_t len2 = std::strlen(str2);
    char* result = new char[len1 + len2 + 1];
    std::strcpy(result, str1);
    std::strcat(result, str2);
    return result;
}

char* generateFileName(const char* base, int iteration) {
    char iteration_str[16];
    std::sprintf(iteration_str, "%d", iteration);

    char* base_with_iteration = concatenateCString(base, "/iteration_");
    char* full_name = concatenateCString(base_with_iteration, iteration_str);
    char* filename = concatenateCString(full_name, ".bmp");

    delete[] base_with_iteration;
    delete[] full_name;

    return filename;
}

bool stringBegginingEquality(char* string_a, char* string_b) {
    int strlen_string = strlen(string_b);
    if (strlen(string_a) < strlen(string_b)) {
        strlen_string = strlen(string_a);
    }
    for (int i = 0; i < strlen_string; i++) {
        if (string_a[i] != string_b[i]) {
            return false;
        }
    }
    return true;
}

bool strictStringEquality(char* string_a, char* string_b) {
    if (strlen(string_a) != strlen(string_b)) {
        return false;
    }
    int string_len = strlen(string_b);
    for (int i = 0; i < string_len; i++) {
        if (string_a[i] != string_b[i]) {
            return false;
        }
    }
    return true;
}

bool cstringIsNumber(char* string) {
    int length = strlen(string);
    if (length == 0) {
        return false;
    }
    for (int i = 0; i < length; i++) {
        if (!isdigit(string[i])) {
            return false;
        }
    }
    return true;
}