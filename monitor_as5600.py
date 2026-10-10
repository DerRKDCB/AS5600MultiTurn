#!/usr/bin/env python3
"""
AS5600 Multi-turn Encoder Monitor - scrolling plot
Shows raw angle (0-4095) and position (0-1023) with bar graphs
"""

import serial
import time
import sys

SERIAL_PORT = '/dev/ttyACM0'
BAUD_RATE = 115200

def draw_bar(value, max_val, width=40, char='█'):
    filled = int((value / max_val) * width)
    return char * filled + '░' * (width - filled)

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.1)
    time.sleep(1)
    print(f"Connected to {SERIAL_PORT} at {BAUD_RATE}", flush=True)
    print("Press Ctrl+C to exit\n", flush=True)
    
    buffer = ""
    while True:
        data = ser.read(ser.in_waiting or 1).decode('utf-8', errors='ignore')
        if not data:
            time.sleep(0.01)
            continue
        buffer += data
        
        while '\n' in buffer:
            line, buffer = buffer.split('\n', 1)
            line = line.strip()
            if not line or not line.startswith('Raw:'):
                continue
            
            # Parse: "Raw: 2663  Angle: 234.05  Pos: 511/1023 (49%)  Turns: 10  Wrap: OFF  Mag: 2110  AGC: 62"
            parts = line.split()
            if len(parts) >= 6:
                try:
                    raw = int(parts[1])
                    pos = int(parts[5].split('/')[0])
                    
                    raw_bar = draw_bar(raw, 4095, 35)
                    pos_bar = draw_bar(pos, 1023, 35)
                    
                    print(f"Raw: {raw:4d}/4095  [{raw_bar}]  Pos: {pos:4d}/1023  [{pos_bar}]", flush=True)
                except (ValueError, IndexError):
                    pass
        
        time.sleep(0.01)

except serial.SerialException as e:
    print(f"Error: {e}", flush=True)
except KeyboardInterrupt:
    print("\nStopped", flush=True)
finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()