#ifndef AS5600_MULTITURN_H
#define AS5600_MULTITURN_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AS5600.h>

class AS5600MultiTurn {
public:
    AS5600MultiTurn(TwoWire& wire = Wire, uint8_t sda = 8, uint8_t scl = 9);
    
    bool begin(uint32_t total_turns = 10);
    void reset(int32_t initial_position = -1);
    
    int32_t getPosition();
    int32_t getRawPosition();
    uint16_t getRawAngle();
    float getAngle();
    uint16_t getMagnitude();
    uint8_t getAGC();
    
    bool update();
    
    int32_t getMaxPosition() const;
    int32_t getTotalRawRange() const;
    
private:
    TwoWire& _wire;
    uint8_t _sda;
    uint8_t _scl;
    Adafruit_AS5600 _as5600;
    
    int32_t _total_raw = 0;
    uint16_t _last_raw = 0;
    bool _first_read = true;
    int32_t _total_raw_range = 40960;
    int32_t _max_position = 1023;
    bool _initialized = false;
};

#endif