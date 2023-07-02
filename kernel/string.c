//
// Created by 胡旭林 on 2023/7/2.
//

#include "include/string.h"

size_t strlen(const char* str) {
    size_t res = 0;
    while (str[res]) res++;
    return res;
}