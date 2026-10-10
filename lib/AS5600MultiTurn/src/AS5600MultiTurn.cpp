#include "AS5600MultiTurn.h"

AS5600MultiTurn::AS5600MultiTurn(TwoWire& wire, uint8_t sda, uint8_t scl)
    : _wire(wire), _sda(sda), _scl(scl), _as5600() {}

bool AS5600MultiTurn::begin(uint32_t total_turns) {
    _wire.begin(_sda, _scl);
    
    if (!_as5600.begin()) {
        return false;
    }
    
    _total_raw_range = (int32_t)total_turns * 4096;
    _max_position = 1023;
    
    _last_raw = _as5600.getRawAngle();
    _total_raw = (int32_t)_last_raw + ((int32_t)total_turns / 2) * 4096;
    _first_read = false;
    _initialized = true;
    
    return true;
}

void AS5600MultiTurn::reset(int32_t initial_position) {
    if (!_initialized) return;
    
    _last_raw = _as5600.getRawAngle();
    if (initial_position >= 0) {
        _total_raw = ((int64_t)initial_position * _total_raw_range) / _max_position;
    } else {
        _total_raw = (int32_t)_last_raw + (_total_raw_range / 2);
    }
    _first_read = false;
}

bool AS5600MultiTurn::update() {
    if (!_initialized) return false;
    
    uint16_t raw = _as5600.getRawAngle();
    
    if (!_first_read) {
        int16_t diff = (int16_t)raw - (int16_t)_last_raw;
        
        if (diff > 2048) {
            diff -= 4096;
        } else if (diff < -2048) {
            diff += 4096;
        }
        
        _total_raw += diff;
        
        if (_total_raw < 0) _total_raw = 0;
        if (_total_raw > _total_raw_range) _total_raw = _total_raw_range;
    }
    
    _last_raw = raw;
    _first_read = false;
    
    return true;
}

int32_t AS5600MultiTurn::getPosition() {
    if (!_initialized) return 0;
    return ((int64_t)_total_raw * _max_position) / _total_raw_range;
}

int32_t AS5600MultiTurn::getRawPosition() {
    return _total_raw;
}

uint16_t AS5600MultiTurn::getRawAngle() {
    return _as5600.getRawAngle();
}

float AS5600MultiTurn::getAngle() {
    uint16_t raw = _as5600.getRawAngle();
    return (raw / 4096.0) * 360.0;
}

uint16_t AS5600MultiTurn::getMagnitude() {
    return _as5600.getMagnitude();
}

uint8_t AS5600MultiTurn::getAGC() {
    return _as5600.getAGC();
}

int32_t AS5600MultiTurn::getMaxPosition() const {
    return _max_position;
}

int32_t AS5600MultiTurn::getTotalRawRange() const {
    return _total_raw_range;
}