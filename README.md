# AS5600MultiTurn

Arduino library for using the AS5600 hall effect magnetic rotary position sensor as a **linear multi-turn encoder**.

## Features

- **Multi-turn tracking**: Tracks absolute position across multiple rotations (default 10 turns = 0-1023)
- **No rollover**: Position clamps at endpoints (0 and max), doesn't wrap around
- **Configurable turns**: Set any number of turns (1 to 32+)
- **Linear output**: 0-1023 range (10-bit) for easy integration
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
    
    // Initialize with 10 turns (default)
    // Custom I2C pins for ESP32-C3 Super Mini
    if (!encoder.begin(10)) {
        Serial.println("AS5600 not found!");
        while (1) delay(1000);
    }
}

void loop() {
    encoder.update();
    
    // Position: 0-1023 (10 turns)
    Serial.print("Position: ");
    Serial.print(encoder.getPosition());
    Serial.print("/1023");
    
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

## API

| Method | Description |
|--------|-------------|
| `begin(turns)` | Initialize with number of turns (default 10) |
| `update()` | Call in loop() to track position |
| `getPosition()` | Current position 0-1023 |
| `getRawPosition()` | Raw accumulator 0 to (turns × 4096) |
| `getRawAngle()` | Single-turn raw angle 0-4095 |
| `getAngle()` | Single-turn angle 0-360° |
| `getMagnitude()` | Magnetic field strength |
| `getAGC()` | Automatic Gain Control value |
| `reset(value)` | Reset position (0-1023, or -1 for middle) |
| `getMaxPosition()` | Returns 1023 |
| `getTotalRawRange()` | Returns turns × 4096 |

## Examples

- `examples/BasicExample` - Simple position reading
- `examples/AdvancedExample` - With serial commands (reset, etc.)

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
5. Maps to 0-1023 output range
6. Clamps at 0 and max (no rollover)

## Wiring (ESP32-C3 Super Mini)

```
AS5600    ESP32-C3
VCC    →  3.3V
GND    →  GND
SDA    →  GPIO 8
SCL    →  GPIO 9
```

For other boards, change pins in `begin()` or use default Wire pins.

## License

MIT