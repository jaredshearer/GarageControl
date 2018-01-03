/*
Garage Control
by Jared Shearer

This Particle sketch integrates a Particle (Photon in my case) with a Relay Shield with SmartThings

The Relay Shield uses D0-D3 to actuate the relays.

The status LED uses PWM to fade an LED to remind you that you left the door open.

*/

int GarageRelay(String command);

int g1Relay = D0;    // relay that activates garage 1, 2, or 3
int g2Relay = D1;
int g3Relay = D2; 
// int g4Relay = D3; // This relay isn't being used...but it could be! You'd just need to edit your GarageRelay() function to include a fourth relay

int g1Switch = D4;   // switch to see if garage 1, 2, or 3 is closed or open
int g2Switch = D5;
int g3Switch = D6;

// Defining stuff for that sweet, sweet PWM'ing status LED
int led = A4;        // define what pin the LED is on (A4 is a PWM pin)
int brightness = 0;  // how bright the LED is (PWM)
int fadeAmount = 5;  // how many points (between 0 and 255) to fade the LED by

// Door state sensing declarations used in the loop and by SmartThings
int g1State = 0;
int g2State = 0;
int g3State = 0;
int g1LastState = 0;
int g2LastState = 0;
int g3LastState = 0;

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
    Particle.variable("g1State", g1State);
    Particle.variable("g2State", g2State);
    Particle.variable("g3State", g3State);
    
    
}

void loop() {
    
    g1State = digitalRead(g1Switch);
    g2State = digitalRead(g2Switch);
    g3State = digitalRead(g3Switch);
    
    // These three if/else statements figure out if the door state has changed since the last 
    // go round, and publish the change in state if they have.
    if(g1State == g1LastState){
        // If they're the same, do nothing.
    }
    else{ // If they're different, figure out which state it's in and publish its state.
        if(g1State == 0){
        Particle.publish("Garage 1 closed.");
        }
        else if(g1State == 1){
        Particle.publish("Garage 1 opened.");
        }        
    }
    
    if(g2State == g2LastState){
        // If they're the same, do nothing.
    }
    else{ // If they're different, figure out which state it's in and publish its state.
        if(g2State == 0){
        Particle.publish("Garage 2 closed.");
        }
        else if(g2State == 1){
        Particle.publish("Garage 2 opened.");
        }        
    }
    
    if(g3State == g3LastState){
        // If they're the same, do nothing.
    }
    else{ // If they're different, figure out which state it's in and publish its state.
        if(g3State == 0){
        Particle.publish("Garage 3 closed.");
        }
        else if(g3State == 1){
        Particle.publish("Garage 3 opened.");
        }        
    }
    
    g1LastState = g1State;
    g2LastState = g2State;
    g3LastState = g3State;
    
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

    // This is the function that gets called from SmartThings to trigger the relays.
    // Notice that it's outside of the loop, and it only runs when it's called.
    int GarageRelay(String command){
        
        if (command == "1") {        // Toggle g1Relay with a single LED flash in the middle for feedback and delay adjusted for 1 second
            digitalWrite(g1Relay, HIGH);
            delay(250);
            digitalWrite(led, HIGH);
            delay(100);
            digitalWrite(led, LOW);
            delay(650);
            digitalWrite(g1Relay, LOW);
            Particle.publish("GarageRelay", "Garage 1 Relay Toggled!", PRIVATE);
	        return 1;
        } 
        
        else if (command == "2"){    // Toggle g2Relay with a double LED flash in the middle for feedback and delay adjusted for 1 second
            digitalWrite(g2Relay, HIGH);
            digitalWrite(led, HIGH);
            delay(250);
            delay(100);
            digitalWrite(led, LOW);
            delay(100);
            digitalWrite(led, HIGH);
            delay(100);
            digitalWrite(led, LOW);
            delay(450);
            digitalWrite(g2Relay, LOW);
            Particle.publish("GarageRelay", "Garage 2 Relay Toggled!", PRIVATE);
            return 2;
        } 
        
        else if (command == "3"){ // Toggle g3Relay with a triple LED flash in the middle for feedback and delay adjusted for 1 second
            digitalWrite(g3Relay, HIGH);
            delay(250);
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
            delay(250);
            digitalWrite(g3Relay, LOW);
            Particle.publish("GarageRelay", "Garage 3 Relay Toggled!", PRIVATE);
	        return 3;
        }
    }
