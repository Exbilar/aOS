//
// Created by 胡旭林 on 2023/7/3.
//

#ifndef AOS_ASSERT_H
#define AOS_ASSERT_H

#include <stdbool.h>

#include "vga.h"

void assert(bool expr);
void assert_write(bool expr, char *ch);

#endif //AOS_ASSERT_H
