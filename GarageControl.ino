int GarageRelay(String command);

int g1Relay = D0;    // relay that activates garage 1, 2, or 3
int g2Relay = D1;
int g3Relay = D2; 
int g1Switch = D4;   // switch to see if garage 1, 2, or 3 is closed or open
int g2Switch = D5;
int g3Switch = D6;

int led = A4;        // define what pin the LED is on (A4 is a PWM pin)
int brightness = 0;  // how bright the LED is (PWM)
int fadeAmount = 5;  // how many points (between 0 and 255) to fade the LED by

void setup() {
    pinMode(g1Relay, OUTPUT);
    pinMode(g2Relay, OUTPUT);
    pinMode(g3Relay, OUTPUT);
    
    pinMode(g1Switch, INPUT);
    pinMode(g2Switch, INPUT);
    pinMode(g3Switch, INPUT);
    
    pinMode(led, OUTPUT);

    digitalWrite(g1Relay, LOW);
    digitalWrite(g2Relay, LOW);
    digitalWrite(g3Relay, LOW);

// Publish toggle functions and door switch variables to the Particle server
    Particle.function("GarageRelay", GarageRelay);
//    Particle.variable("GarageState", GarageState);
    
}

void loop() {
    
    // LED fading in and out code
    // set brightness of led
    if (/*digitalRead(g1Switch) == LOW && digitalRead(g2Switch) == LOW && */digitalRead(g3Switch) == LOW){      // Just using the one switch for now
        analogWrite(led, 0); // if g1Switch is low, turn off the LED
    
    }
    
    else analogWrite(led, brightness);                                      // if it isn't low, fade the LED in and out
        // change brightness for next time through loop
        brightness = brightness + fadeAmount;
        // reverse the direction of the fading when at the ends of the fade
        if (brightness == 0 || brightness == 255) {
            fadeAmount = -fadeAmount;
        }
        // wait to see the dimming effect
        // less is faster
        delay(30);
}

    int GarageRelay(String command){
        
        if (command == "1") {        // Toggle g1Relay with a single LED flash in the middle for feedback and delay adjusted for 1 second
            digitalWrite(g1Relay, HIGH);
            digitalWrite(led, HIGH);
            delay(100);
            digitalWrite(led, LOW);
            delay(900);
            digitalWrite(g1Relay, LOW);
            Particle.publish("GarageRelay", "Garage 1 Relay Toggled!", PRIVATE);
	        return 1;
        } 
        
        else if (command == "2"){    // Toggle g2Relay with a double LED flash in the middle for feedback and delay adjusted for 1 second
            digitalWrite(g2Relay, HIGH);
            digitalWrite(led, HIGH);
            delay(100);
            digitalWrite(led, LOW);
            delay(100);
            digitalWrite(led, HIGH);
            delay(100);
            digitalWrite(led, LOW);
            delay(700);
            digitalWrite(g2Relay, LOW);
            Particle.publish("GarageRelay", "Garage 2 Relay Toggled!", PRIVATE);
            return 2;
        } 
        
        else if (command == "3"){ // Toggle g3Relay with a triple LED flash in the middle for feedback and delay adjusted for 1 second
            digitalWrite(g3Relay, HIGH);
            digitalWrite(led, HIGH);
            delay(100);
            digitalWrite(led, LOW);
            delay(100);
            digitalWrite(led, HIGH);
            delay(100);
            digitalWrite(led, LOW);
            delay(100);
            digitalWrite(led, HIGH);
            delay(100);
            digitalWrite(led, LOW);
            delay(500);
            digitalWrite(g3Relay, LOW);
            Particle.publish("GarageRelay", "Garage 3 Relay Toggled!", PRIVATE);
	        return 3;
        }
    }
