#include "AS5600MultiTurn.h"

AS5600MultiTurn::AS5600MultiTurn(TwoWire& wire, uint8_t sda, uint8_t scl)
    : _wire(wire), _sda(sda), _scl(scl), _as5600(), _config() {}

bool AS5600MultiTurn::begin(const AS5600MultiTurnConfig& config) {
    _config = config;
    return begin(config.total_turns);
}

bool AS5600MultiTurn::begin(uint32_t total_turns) {
    _config.total_turns = total_turns;
    _wire.begin(_sda, _scl);
    
    if (!_as5600.begin()) {
        return false;
    }
    
    _total_raw = 0;
    _last_raw = _as5600.getRawAngle();
    _first_read = false;
    _initialized = true;
    
    reset(_config.start_position);
    
    return true;
}

void AS5600MultiTurn::setConfig(const AS5600MultiTurnConfig& config) {
    _config = config;
    if (_initialized) {
        begin(config);
    }
}

void AS5600MultiTurn::reset(int32_t initial_position) {
    if (!_initialized) return;
    
    _last_raw = _as5600.getRawAngle();
    
    if (initial_position >= 0) {
        int32_t range = _config.output_max - _config.output_min;
        _total_raw = ((int64_t)(initial_position - _config.output_min) * _config.total_turns * 4096) / range;
    } else {
        _total_raw = (_config.total_turns * 4096) / 2;
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
        _total_raw = _clampOrWrap(_total_raw);
    }
    
    _last_raw = raw;
    _first_read = false;
    
    return true;
}

int32_t AS5600MultiTurn::_mapToOutput(int32_t raw) const {
    int64_t range = (int64_t)_config.total_turns * 4096;
    int64_t output_range = (int64_t)_config.output_max - _config.output_min;
    return _config.output_min + (raw * output_range) / range;
}

int32_t AS5600MultiTurn::_clampOrWrap(int32_t value) const {
    int32_t min_raw = 0;
    int32_t max_raw = _config.total_turns * 4096;
    
    if (_config.wrap_around) {
        int32_t range = max_raw - min_raw;
        if (value < min_raw) value = max_raw - ((min_raw - value) % range);
        if (value > max_raw) value = min_raw + ((value - max_raw) % range);
    } else {
        if (value < min_raw) value = min_raw;
        if (value > max_raw) value = max_raw;
    }
    return value;
}

int32_t AS5600MultiTurn::getPosition() {
    if (!_initialized) return _config.output_min;
    return _mapToOutput(_total_raw);
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

int32_t AS5600MultiTurn::getMaxPosition() {
    return _config.output_max;
}

int32_t AS5600MultiTurn::getMinPosition() {
    return _config.output_min;
}

int32_t AS5600MultiTurn::getTotalRawRange() {
    return _config.total_turns * 4096;
}

bool AS5600MultiTurn::getWrapAround() {
    return _config.wrap_around;
}

uint32_t AS5600MultiTurn::getTotalTurns() {
    return _config.total_turns;
}