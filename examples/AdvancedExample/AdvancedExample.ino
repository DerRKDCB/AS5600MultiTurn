#include <AS5600MultiTurn.h>

AS5600MultiTurn encoder;

int32_t last_position = -1;
uint32_t last_print = 0;

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(100);
    delay(500);
    
    Serial.println("AS5600 Multi-turn Encoder - Advanced Example");
    Serial.println("Features: position tracking, change detection, reset");
    Serial.println("Commands: 'r' = reset to middle, '0' = reset to 0, 'm' = reset to max");
    Serial.println("---");
    
    if (!encoder.begin(10)) {
        Serial.println("ERROR: Failed to initialize AS5600!");
        while (1) delay(1000);
    }
    
    Serial.println("Ready. Turn the knob.");
    Serial.println();
}

void loop() {
    encoder.update();
    
    int32_t pos = encoder.getPosition();
    
    if (pos != last_position) {
        uint32_t now = millis();
        Serial.print("[");
        Serial.print(now);
        Serial.print("ms] Position: ");
        Serial.print(pos);
        Serial.print("/");
        Serial.print(encoder.getMaxPosition());
        Serial.print(" (");
        Serial.print((pos * 100) / encoder.getMaxPosition());
        Serial.print("%)  Raw: ");
        Serial.print(encoder.getRawAngle());
        Serial.print("  Mag: ");
        Serial.println(encoder.getMagnitude());
        last_position = pos;
    }
    
    if (Serial.available()) {
        char cmd = Serial.read();
        switch (cmd) {
            case 'r':
            case 'R':
                encoder.reset();
                Serial.println("Reset to middle (512)");
                last_position = -1;
                break;
            case '0':
                encoder.reset(0);
                Serial.println("Reset to 0");
                last_position = -1;
                break;
            case 'm':
            case 'M':
                encoder.reset(encoder.getMaxPosition());
                Serial.println("Reset to max (1023)");
                last_position = -1;
                break;
        }
    }
    
    delay(10);
}