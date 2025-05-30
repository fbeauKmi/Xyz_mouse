// struct.h structure for Orbion

#pragma once
#ifndef STRUCT_H
#define STRUCT_H

typedef struct
{
  uint32_t color1;
  uint32_t color2;
  uint16_t timeout;
  uint8_t Mode;
  int8_t Encoder;  
  uint8_t led_mode;
  uint8_t led_color_mode;
  uint8_t contrast;
  uint8_t dz;
  uint8_t sensitivity;
  
} MenuConfig;

typedef struct {
    int16_t x, y, z;
} Axes;

// Add operator+ after the type is defined
inline Axes operator+(const Axes& a, const Axes& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

inline Axes operator<<(const Axes& a, uint8_t b) {
    return {a.x << b , a.y << b, a.z << b};
}

inline Axes operator>>(const Axes& a, uint8_t b) {
    return {a.x >> b , a.y >> b, a.z >> b};
}

#endif