#include <AS5600MultiTurn.h>

AS5600MultiTurn encoder;

int32_t last_position = -1;
uint32_t last_print = 0;

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(100);
    delay(500);
    
    Serial.println("AS5600 Multi-turn Encoder - Advanced Example");
    Serial.println("Features: position tracking, change detection, runtime config");
    Serial.println("Commands:");
    Serial.println("  'r' = reset to middle, '0' = reset to min, 'm' = reset to max");
    Serial.println("  'w' = toggle wrap/clamp, '+'/'-' = change turns");
    Serial.println("---");
    
    AS5600MultiTurnConfig config;
    config.total_turns = 10;
    config.output_min = 0;
    config.output_max = 1023;
    config.wrap_around = false;
    config.start_position = 512;
    
    if (!encoder.begin(config)) {
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
        AS5600MultiTurnConfig config;
        config.total_turns = encoder.getTotalTurns();
        config.output_min = encoder.getMinPosition();
        config.output_max = encoder.getMaxPosition();
        config.wrap_around = encoder.getWrapAround();
        
        switch (cmd) {
            case 'r': case 'R':
                config.start_position = (config.output_max + config.output_min) / 2;
                encoder.setConfig(config);
                Serial.println("Reset to middle");
                last_position = -1;
                break;
            case '0':
                config.start_position = config.output_min;
                encoder.setConfig(config);
                Serial.println("Reset to min");
                last_position = -1;
                break;
            case 'm': case 'M':
                config.start_position = config.output_max;
                encoder.setConfig(config);
                Serial.println("Reset to max");
                last_position = -1;
                break;
            case 'w': case 'W':
                config.wrap_around = !config.wrap_around;
                encoder.setConfig(config);
                Serial.print("Wrap: ");
                Serial.println(config.wrap_around ? "ON" : "OFF");
                break;
            case '+':
                if (config.total_turns < 32) {
                    config.total_turns++;
                    encoder.setConfig(config);
                    Serial.print("Turns: ");
                    Serial.println(config.total_turns);
                }
                break;
            case '-':
                if (config.total_turns > 1) {
                    config.total_turns--;
                    encoder.setConfig(config);
                    Serial.print("Turns: ");
                    Serial.println(config.total_turns);
                }
                break;
        }
    }
    
    delay(10);
}