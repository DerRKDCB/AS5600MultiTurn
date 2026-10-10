#include <AS5600MultiTurn.h>

AS5600MultiTurn encoder;

void setup() {
    Serial.begin(115200);
    
    while (!Serial) {
        delay(100);
    }
    
    delay(500);
    
    Serial.println("AS5600 Multi-turn Encoder - Configuration Demo");
    Serial.println("=============================================");
    
    AS5600MultiTurnConfig config;
    config.total_turns = 10;
    config.output_min = 0;
    config.output_max = 1023;
    config.wrap_around = false;
    config.start_position = 512;
    
    Serial.println("Settings:");
    Serial.print("  Turns: ");
    Serial.println(config.total_turns);
    Serial.print("  Output range: ");
    Serial.print(config.output_min);
    Serial.print(" - ");
    Serial.println(config.output_max);
    Serial.print("  Wrap around: ");
    Serial.println(config.wrap_around ? "YES" : "NO (clamp)");
    Serial.print("  Start position: ");
    Serial.println(config.start_position);
    Serial.println();
    
    if (!encoder.begin(config)) {
        Serial.println("ERROR: AS5600 not found!");
        while (1) delay(1000);
    }
    
    Serial.println("Encoder ready!");
    Serial.println("Commands:");
    Serial.println("  'r' - reset to middle");
    Serial.println("  '0' - reset to min");
    Serial.println("  'm' - reset to max");
    Serial.println("  'w' - toggle wrap/clamp");
    Serial.println("  '+' - increase turns");
    Serial.println("  '-' - decrease turns");
    Serial.println();
}

void loop() {
    encoder.update();
    
    Serial.print("Raw: ");
    Serial.print(encoder.getRawAngle());
    Serial.print("  Angle: ");
    Serial.print(encoder.getAngle(), 2);
    Serial.print("  Pos: ");
    Serial.print(encoder.getPosition());
    Serial.print("/");
    Serial.print(encoder.getMaxPosition());
    Serial.print(" (");
    Serial.print((encoder.getPosition() * 100) / encoder.getMaxPosition());
    Serial.print("%)  Turns: ");
    Serial.print(encoder.getTotalTurns());
    Serial.print("  Wrap: ");
    Serial.print(encoder.getWrapAround() ? "ON" : "OFF");
    Serial.print("  Mag: ");
    Serial.print(encoder.getMagnitude());
    Serial.print("  AGC: ");
    Serial.println(encoder.getAGC());
    
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
                break;
            case '0':
                config.start_position = config.output_min;
                encoder.setConfig(config);
                Serial.println("Reset to min");
                break;
            case 'm': case 'M':
                config.start_position = config.output_max;
                encoder.setConfig(config);
                Serial.println("Reset to max");
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
    
    delay(50);
}