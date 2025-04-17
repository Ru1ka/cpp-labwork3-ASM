#pragma once
#include <cstring>
#include <iostream>

char* concatenateCString(const char* str1, const char* str2);

char* generateFileName(const char* base, int iteration);

bool stringBegginingEquality(char* string_a, char* string_b);

bool strictStringEquality(char* string_a, char* string_b);

bool cstringIsNumber(char* string);