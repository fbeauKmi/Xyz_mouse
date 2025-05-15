
#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

bool isTimeout(uint32_t* lastMillis, uint32_t delay);
bool isTimeout(uint32_t* lastMillis, uint32_t* _currentMillis, uint32_t delay, bool reset);
#endif