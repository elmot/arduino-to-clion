//
// Created by Ilia.Motornyi on 30/04/2020.
//
#include "main.h"
#include <cstdio>

__attribute__((noreturn))
void cppMain() {
  while(true) {
    printf("Hello, World\r\n");
    HAL_Delay(500);
  }
}