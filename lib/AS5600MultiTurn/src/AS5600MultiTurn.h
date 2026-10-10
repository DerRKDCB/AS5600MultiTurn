#ifndef AS5600_MULTITURN_H
#define AS5600_MULTITURN_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AS5600.h>

struct AS5600MultiTurnConfig {
    uint32_t total_turns = 10;
    int32_t output_min = 0;
    int32_t output_max = 1023;
    bool wrap_around = false;
    int32_t start_position = -1;
};

class AS5600MultiTurn {
public:
    AS5600MultiTurn(TwoWire& wire = Wire, uint8_t sda = 8, uint8_t scl = 9);
    
    bool begin(const AS5600MultiTurnConfig& config = AS5600MultiTurnConfig());
    bool begin(uint32_t total_turns);
    void reset(int32_t initial_position = -1);
    void setConfig(const AS5600MultiTurnConfig& config);
    
    int32_t getPosition();
    int32_t getRawPosition();
    uint16_t getRawAngle();
    float getAngle();
    uint16_t getMagnitude();
    uint8_t getAGC();
    
    bool update();
    
    int32_t getMaxPosition();
    int32_t getMinPosition();
    int32_t getTotalRawRange();
    bool getWrapAround();
    uint32_t getTotalTurns();
    
private:
    TwoWire& _wire;
    uint8_t _sda;
    uint8_t _scl;
    Adafruit_AS5600 _as5600;
    
    AS5600MultiTurnConfig _config;
    
    int32_t _total_raw = 0;
    uint16_t _last_raw = 0;
    bool _first_read = true;
    bool _initialized = false;
    
    int32_t _mapToOutput(int32_t raw) const;
    int32_t _clampOrWrap(int32_t value) const;
};

#endif