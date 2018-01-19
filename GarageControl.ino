/*
**
** Garage Control
**
** Copyright 2018 by Jared Shearer
**
** Many thanks to bscuderi13 for all of the guidance.
**
** This Particle sketch integrates a Particle (Photon in my case) with a 
** Relay Shield with SmartThings.
**
** WebCoRE is used to refresh the state of the garage doors in SmartThings 
** on a particle.publish from the garage state logic in the loop. 
**
** The Particle Relay Shield uses D3-D6 to control the relays.
**
** The status LED uses PWM to fade an LED to remind you that you left the 
** door open because extra fancy.  Note: Only some pins are PWM capable.  A4 is one.
**
*/

// Define all the thingz
int GarageRelay(String command);  // This defines the function that will actually toggle the relays to open the doors.

int g1Relay = D3;    // relay that activates garage 1, 2, or 3
int g2Relay = D4;
int g3Relay = D5; 
// int g4Relay = D6; // This relay isn't being used...but it could be!

int g1Switch = D0;   // switch to see if garage 1, 2, or 3 is closed or open
int g2Switch = D1;
int g3Switch = D2;

// Door state sensing declarations used in the loop and by SmartThings
String g1Status = "unknown";
String g2Status = "unknown";
String g3Status = "unknown";
int g1LastState = -1;  // Set last state to -1 so that the "if state
int g2LastState = -1;  // doesn't equal laststate" functions from below will  
int g3LastState = -1;  // absolutely get tripped on a device restart

// Defining stuff for that sweet, sweet PWM'ing status LED
int led = A4;        // define what pin the LED is on (A4 is a PWM pin)
int brightness = 0;  // how bright the LED is (PWM)
int fadeAmount = 5;  // how many points (between 0 and 255) to fade the LED by

void setup() {
    pinMode(g1Relay, OUTPUT);
    pinMode(g2Relay, OUTPUT);
    pinMode(g3Relay, OUTPUT);
    
    pinMode(g1Switch, INPUT_PULLUP);  // Set the switches to input and pull them up so that
    pinMode(g2Switch, INPUT_PULLUP);  // they read HIGH if they aren't connected to ground
    pinMode(g3Switch, INPUT_PULLUP);  // AKA "You no-a float!"
    
    pinMode(led, OUTPUT);

    digitalWrite(g1Relay, LOW);  // Set your relays to low so we don't accidentally open the doors on restart.
    digitalWrite(g2Relay, LOW);  // That would tank the WAF.
    digitalWrite(g3Relay, LOW);

// Publish toggle function and door switch variables to the Particle server so SmartThings can see them
    Particle.function("GarageRelay", GarageRelay);
    Particle.variable("g1Status", g1Status);  // Variable names can only be a max of twelve characters!
    Particle.variable("g2Status", g2Status);
    Particle.variable("g3Status", g3Status);
    
}

void loop() {
    
    int g1State = digitalRead(g1Switch);  // Read the state of each door from its respective variable..over and over and over
    int g2State = digitalRead(g2Switch);  // and store it to the respective variable
    int g3State = digitalRead(g3Switch);
    
    // These three if/else statements figure out if the door state has changed since the last 
    // go round, and publish the change in state if they have.  That publish triggers a 
    // WebCoRE piston (awesome!) that refreshes door status in SmartThings.
    if(g1State == g1LastState){
        // If they're the same, do nothing.
    }
    else{ // If they're different, figure out which state it's in and publish its state.
        if(g1State == HIGH){
            g1Status = "Open";
            Particle.publish("Garage 1", "Opened");
        }
        else if(g1State == LOW){
            g1Status = "Closed";
            Particle.publish("Garage 1", "Closed");
        }        
    }
    
    if(g2State == g2LastState){
        // If they're the same, do nothing.
    }
    else{ // If they're different, figure out which state it's in and publish its state.
        if(g2State == HIGH){
            g2Status = "Open";
            Particle.publish("Garage 2", "Opened");
        }
        else if(g2State == LOW){
            g2Status = "Closed";
            Particle.publish("Garage 2", "Closed");
        }        
    }
    
    if(g3State == g3LastState){
        // If they're the same, do nothing.
    }
    else{ // If they're different, figure out which state it's in and publish its state.
        if(g3State == HIGH){
            g3Status = "Open";
            Particle.publish("Garage 3", "Opened");
        }
        else if(g3State == LOW){
            g3Status = "Closed";
            Particle.publish("Garage 3", "Closed");
        }        
    }
    
    // These set the last states to the current states for the next go round.
    g1LastState = g1State;
    g2LastState = g2State;
    g3LastState = g3State;

    
    
    // LED fading in and out code to remind you that you left the garage door open
    // set brightness of led
    if (/*digitalRead(g1Switch) == LOW && digitalRead(g2Switch) == LOW && */digitalRead(g3Switch) == LOW){  // Just using the one switch for now
        analogWrite(led, 0);  // If g3Switch is low, turn off the LED.
    
    }
    
    else analogWrite(led, brightness);  // If it isn't low, fade the LED in and out to tell the wife I left the garage door open...again.
        // Change brightness for next time through loop
        brightness = brightness + fadeAmount;
        // Reverse the direction of the fading when at the ends of the fade
        if (brightness == 0 || brightness == 255) {
            fadeAmount = -fadeAmount;
        }
        // Wait to see the dimming effect
        // Less is faster
        delay(30);
} // END LOOP



    // This is the function that gets called from SmartThings to trigger the relays.
    // It's outside of the loop, so it only runs when it's called.
    int GarageRelay(String command){
        
        if (command == "1") {  // Toggle g1Relay with a single LED flash in the middle for feedback and delay adjusted for a 1 second toggle
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
        
        else if (command == "2"){  // Toggle g2Relay with a double LED flash in the middle for feedback and delay adjusted for a 1 second toggle
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
        
        else if (command == "3"){  // Toggle g3Relay with a triple LED flash in the middle for feedback and delay adjusted for a 1 second toggle
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
