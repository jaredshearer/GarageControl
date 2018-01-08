/*
Garage Control

Copyright 2018 by jsheahawk

This Particle sketch integrates a Particle (Photon in my case) with a Relay Shield with SmartThings.

The Relay Shield uses D3-D06 to actuate the relays.

The status LED uses PWM to fade an LED to remind you that you left the door open because extra fancy.  Note: Only some pins are PWM capable.  A4 is one.

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

// Defining stuff for that sweet, sweet PWM'ing status LED
int led = A4;        // define what pin the LED is on (A4 is a PWM pin)
int brightness = 0;  // how bright the LED is (PWM)
int fadeAmount = 5;  // how many points (between 0 and 255) to fade the LED by

// Door state sensing declarations used in the loop and by SmartThings
String g1Status = "unknown";
String g2Status = "unknown";
String g3Status = "unknown";
//int g1State = 0;
//int g2State = 0;
//int g3State = 0;
int g1LastState = 0;
int g2LastState = 0;
int g3LastState = 0;

void setup() {
    pinMode(g1Relay, OUTPUT);
    pinMode(g2Relay, OUTPUT);
    pinMode(g3Relay, OUTPUT);
    
    pinMode(g1Switch, INPUT_PULLDOWN);
    pinMode(g2Switch, INPUT_PULLDOWN);
    pinMode(g3Switch, INPUT_PULLDOWN);
    
    pinMode(led, OUTPUT);

    digitalWrite(g1Relay, LOW); // Set your relays to low so we don't accidentally open the doors on restart
    digitalWrite(g2Relay, LOW);
    digitalWrite(g3Relay, LOW);

// Publish toggle functions and door switch variables to the Particle server so SmartThings can see them
    Particle.function("GarageRelay", GarageRelay);
    Particle.variable("g1Status", g1Status);
    Particle.variable("g2Status", g2Status);
    Particle.variable("g3Status", g3Status);
    
}

void loop() {
    
    int g1State = digitalRead(g1Switch); // Read the state of each door from its respective variable..over and over and over
    int g2State = digitalRead(g2Switch);
    int g3State = digitalRead(g3Switch);
    
    //int status = digitalRead(microswitch); // read the microswitch pin and store the value to status
    if (g1State == HIGH){                   // check status if its high store open in the garageContact string
        g1Status = "Open";
    }
    if (g1State == LOW){                   // check status if its low store closed in the garageContact string 
        g1Status = "Closed";
    }
    
    if (g2State == HIGH){                   // check status if its high store open in the garageContact string
        g2Status = "Open";
    }
    if (g2State == LOW){                   // check status if its low store closed in the garageContact string 
        g2Status = "Closed";
    }
    
    if (g3State == HIGH){                   // check status if its high store open in the garageContact string
        g3Status = "Open";
    }
    if (g3State == LOW){                   // check status if its low store closed in the garageContact string 
        g3Status = "Closed";
    }
    
    
    /* COMMENTING THIS OUT FOR NOW.  GET LAST STATE UPDATING WORKING LATER
    // These three if/else statements figure out if the door state has changed since the last 
    // go round, and publish the change in state if they have.
    if(g1State == g1LastState){
        // If they're the same, do nothing.
    }
    else{ // If they're different, figure out which state it's in and publish its state.
        if(g1State == 0){
        Particle.publish("Garage 1", "Closed");
        //Particle.variable("g1State", 0);
        }
        else if(g1State == 1){
        Particle.publish("Garage 1", "Opened");
        //Particle.variable("g1State", 1);
        }        
    }
    
    if(g2State == g2LastState){
        // If they're the same, do nothing.
    }
    else{ // If they're different, figure out which state it's in and publish its state.
        if(g2State == 0){
        Particle.publish("Garage 2", "Closed");
        //Particle.variable("g2State", 0);
        }
        else if(g2State != 0){
        Particle.publish("Garage 2", "Opened");
        //Particle.variable("g2State", 1);
        }        
    }
    
    if(g3State == g3LastState){
        // If they're the same, do nothing.
    }
    else{ // If they're different, figure out which state it's in and publish its state.
        if(g3State == 0){
        Particle.publish("Garage 3", "Closed");
        //Particle.variable("g3State", 0);
        }
        else if(g3State == 1){
        Particle.publish("Garage 3", "Opened");
        //Particle.variable("g3State", 1);
        }        
    }
    
    
    
    // These set the last states to the current states for the next go round.
    g1LastState = g1State;
    g2LastState = g2State;
    g3LastState = g3State;
    */
    
    
    // LED fading in and out code to remind you that you left the garage door open
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
} // END LOOP



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
