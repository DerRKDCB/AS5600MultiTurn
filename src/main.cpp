#include <AS5600MultiTurn.h>

AS5600MultiTurn encoder;

void setup() {
    Serial.begin(115200);
    
    while (!Serial) {
        delay(100);
    }
    
    delay(500);
    
    Serial.println("AS5600 Multi-turn Encoder Library Demo");
    Serial.println("I2C: SDA=8, SCL=9 | 10 turns = 0-1023");
    Serial.println("---");
    
    AS5600MultiTurnConfig config;
    config.total_turns = 10;
    config.output_min = 0;
    config.output_max = 1023;
    config.wrap_around = false;
    config.start_position = 512;
    
    if (!encoder.begin(config)) {
        Serial.println("ERROR: AS5600 not found!");
        while (1) delay(1000);
    }
    
    Serial.println("Library initialized!");
    Serial.println("Turn knob to test (10 turns = 0-1023)");
    Serial.println("Clamps at endpoints, no rollover");
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
    Serial.print("  RawPos: ");
    Serial.print(encoder.getRawPosition());
    Serial.print("/");
    Serial.print(encoder.getTotalRawRange());
    Serial.print("  Mag: ");
    Serial.print(encoder.getMagnitude());
    Serial.print("  AGC: ");
    Serial.println(encoder.getAGC());
    
    delay(50);
}