#include <AS5600MultiTurn.h>

AS5600MultiTurn encoder;

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(100);
    delay(500);
    
    Serial.println("AS5600 Multi-turn Encoder - Basic Example");
    
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
    
    Serial.println("Encoder ready!");
    Serial.print("Max position: ");
    Serial.println(encoder.getMaxPosition());
    Serial.print("Total raw range: ");
    Serial.println(encoder.getTotalRawRange());
    Serial.println();
}

void loop() {
    encoder.update();
    
    Serial.print("Raw angle: ");
    Serial.print(encoder.getRawAngle());
    Serial.print("  Angle: ");
    Serial.print(encoder.getAngle(), 2);
    Serial.print(" deg");
    Serial.print("  Position: ");
    Serial.print(encoder.getPosition());
    Serial.print("/");
    Serial.print(encoder.getMaxPosition());
    Serial.print("  Raw pos: ");
    Serial.print(encoder.getRawPosition());
    Serial.print("/");
    Serial.print(encoder.getTotalRawRange());
    Serial.print("  Mag: ");
    Serial.print(encoder.getMagnitude());
    Serial.print("  AGC: ");
    Serial.println(encoder.getAGC());
    
    delay(50);
}