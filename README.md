# AS5600MultiTurn

Arduino library for using the AS5600 hall effect magnetic rotary position sensor as a **linear multi-turn encoder**.

## Features

- **Multi-turn tracking**: Tracks absolute position across multiple rotations (default 10 turns = 0-1023)
- **Configurable behavior**: Clamp at endpoints or wrap around (overflow)
- **Configurable turns**: Set any number of turns (1 to 32+)
- **Configurable output range**: Any min/max range (default 0-1023)
- **Runtime configuration**: Change settings on the fly
- **Raw access**: Get raw angle (0-4095), angle in degrees, magnitude, AGC
- **Reset capability**: Reset position to any value at runtime

## Hardware

- AS5600 sensor (I2C address 0x36)
- ESP32-C3 / ESP32 / Arduino / any Arduino-compatible board
- I2C connections: SDA, SCL, VCC (3.3V/5V), GND

Default pins for ESP32-C3 Super Mini:
- SDA: GPIO 8
- SCL: GPIO 9

## Installation

### PlatformIO (from GitHub)

Add to your `platformio.ini`:

```ini
[env:your_board]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino
lib_deps =
    https://github.com/DerRKDCB/AS5600MultiTurn.git
    adafruit/Adafruit AS5600 Library@^1.0.0
```

### Arduino IDE

1. Sketch → Include Library → Manage Libraries
2. Search "AS5600MultiTurn" (once published to library registry)
3. Or: Sketch → Include Library → Add .ZIP Library → select downloaded repo

### Manual

Copy `lib/AS5600MultiTurn` to your project's `lib/` folder.

## Quick Start

```cpp
#include <AS5600MultiTurn.h>

AS5600MultiTurn encoder;

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(100);
    
    // Initialize with default config (10 turns, 0-1023, clamp)
    if (!encoder.begin()) {
        Serial.println("AS5600 not found!");
        while (1) delay(1000);
    }
}

void loop() {
    encoder.update();
    
    // Position: 0-1023 (10 turns)
    Serial.print("Position: ");
    Serial.print(encoder.getPosition());
    Serial.print("/");
    Serial.print(encoder.getMaxPosition());
    
    // Raw angle: 0-4095 (single turn)
    Serial.print("  Raw: ");
    Serial.print(encoder.getRawAngle());
    
    // Angle in degrees
    Serial.print("  Angle: ");
    Serial.print(encoder.getAngle(), 1);
    Serial.println("°");
    
    delay(50);
}
```

## Configuration

### Full Configuration Struct

```cpp
AS5600MultiTurnConfig config;
config.total_turns = 10;       // Number of turns to track (1-32+)
config.output_min = 0;         // Minimum output value
config.output_max = 1023;      // Maximum output value
config.wrap_around = false;    // true = overflow/wrap, false = clamp at endpoints
config.start_position = 512;   // Initial position (-1 = middle of range)

encoder.begin(config);
```

### Runtime Configuration Changes

```cpp
// Get current config, modify, apply
AS5600MultiTurnConfig config;
config.total_turns = encoder.getTotalTurns();
config.output_min = encoder.getMinPosition();
config.output_max = encoder.getMaxPosition();
config.wrap_around = encoder.getWrapAround();
config.start_position = 0;  // Reset to minimum

encoder.setConfig(config);
```

### Config Options

| Parameter | Default | Description |
|-----------|---------|-------------|
| `total_turns` | 10 | Number of full rotations to track |
| `output_min` | 0 | Minimum output value |
| `output_max` | 1023 | Maximum output value |
| `wrap_around` | false | `true` = wrap/overflow, `false` = clamp at endpoints |
| `start_position` | -1 | Initial position (-1 = middle) |

## API

| Method | Description |
|--------|-------------|
| `begin(config)` | Initialize with config struct |
| `begin(turns)` | Simple init with just turn count |
| `setConfig(config)` | Update configuration at runtime |
| `reset(value)` | Reset position to value (or middle if -1) |
| `update()` | Call in loop() to track position |
| `getPosition()` | Current mapped position (output_min to output_max) |
| `getRawPosition()` | Raw accumulator (0 to turns × 4096) |
| `getRawAngle()` | Single-turn raw angle (0-4095) |
| `getAngle()` | Single-turn angle in degrees (0-360°) |
| `getMagnitude()` | Magnetic field strength |
| `getAGC()` | Automatic Gain Control value |
| `getMaxPosition()` | Returns output_max |
| `getMinPosition()` | Returns output_min |
| `getTotalRawRange()` | Returns turns × 4096 |
| `getWrapAround()` | Returns wrap_around setting |
| `getTotalTurns()` | Returns total_turns |

## Examples

- `examples/BasicExample` - Simple position reading with config
- `examples/AdvancedExample` - Runtime config changes via serial commands

Run with:
```bash
platformio run -t upload
platformio device monitor
```

## Monitor Script

Real-time terminal monitor with scrolling bar graphs:

```bash
python3 monitor_as5600.py
```

Shows:
- Raw angle (0-4095) with bar
- Position (0-1023) with bar
- Raw position (0-40960) with bar

## How It Works

The AS5600 provides a 12-bit angle (0-4095) per turn. The library:
1. Reads the raw angle on each update
2. Calculates the difference from previous reading
3. Handles wraparound at 0/4095 boundary
4. Accumulates total raw position across turns
5. Maps to configured output range
6. Clamps or wraps at endpoints based on `wrap_around` setting

## Wiring (ESP32-C3 Super Mini)

```
AS5600    ESP32-C3
VCC    →  3.3V
GND    →  GND
SDA    →  GPIO 8
SCL    →  GPIO 9
```

For other boards, change pins in constructor or use default Wire pins.

```cpp
// Custom I2C pins
AS5600MultiTurn encoder(Wire, 8, 9);  // SDA, SCL
```

## License

MIT