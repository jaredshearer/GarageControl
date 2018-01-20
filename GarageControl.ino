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
** After all this, integration with Google and Alexa via IFTTT is trivial.
**
** The Particle Relay Shield uses D3-D6 to control the relays, but we're only 
** using D3-D5.  You could add another if you have four doors.
**
** We have a LED installed in our bedroom that flashes if a door is open.  The wire loves it. 
** Thanks to a fancy RGB common cathode(-) LEDs, it will flash R for garage 1, G for garage 2,
** and B for garage 3 if it's open.  I used pins A4, A5, and A7 because they each go from 0 to 255.
** Other analog pins went up to 4000-something.  There was some reason for this.  The common goes to ground.
**
** GarageRelay commands are:
**  - "1" -- toggle garage 1
**  - "2" -- toggle garage 2
**  - "3" -- toggle garage 3
**  - "open1" -- open garage 1 only if it's currently closed
**  - "close1" -- close garage 1 only if it's currently open
**  - "open2" -- open garage 2 only if it's currently closed
**  - "close2" -- close garage 2 only if it's currently open
**  - "open3" -- open garage 3 only if it's currently closed
**  - "close3" -- close garage 3 only if it's currently open
**  - "openAll" -- trigger all doors only if they're closed
**  - "closeAll" -- trigger all doors only if they're open
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
String g1Status = "unknown";  // We set the state of these based on whether
String g2Status = "unknown";  // g#State and g#LastState are the same or 
String g3Status = "unknown";  // different
int g1LastState = -1;  // Set last state to -1 so that the "if state
int g2LastState = -1;  // doesn't equal laststate" functions from below will  
int g3LastState = -1;  // absolutely get tripped on a device restart

// Defining stuff the flashing status LED
int rLED = A4;  // define what pins of the RGB LED legs
int gLED = A5;
int bLED = A7;
int triggerLED = D7; // This onboard LED flashes when a door is triggered
int brightness = 10;  // how bright the LED flashing LED is
int ledDelay = 500;  // RGB LED flash time

void setup() {
    pinMode(g1Relay, OUTPUT);
    pinMode(g2Relay, OUTPUT);
    pinMode(g3Relay, OUTPUT);
    
    pinMode(g1Switch, INPUT_PULLUP);  // Set the switches to input and pull them up so that
    pinMode(g2Switch, INPUT_PULLUP);  // they read HIGH if they aren't connected to ground
    pinMode(g3Switch, INPUT_PULLUP);  // AKA "You no-a float!"
    
    pinMode(rLED, OUTPUT);  // Set the LED pins to output
    pinMode(gLED, OUTPUT);
    pinMode(bLED, OUTPUT);
    pinMode(triggerLED, OUTPUT);
    
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
    int g2State = digitalRead(g2Switch);  // and store it to its respective g#State variable
    int g3State = digitalRead(g3Switch);
    
    // These three if/else statements figure out if the door state has changed since the last 
    // go round and publish the change in state if they have.  That publish triggers a 
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

    
    // This block of code flashes the RGB LED if one or more of the garage doors are open
    if (digitalRead(g1Switch) == LOW){
        analogWrite(rLED, 0);
    }
        else{
            analogWrite(rLED, brightness);
            delay(ledDelay);
            analogWrite(rLED,0);
            delay(ledDelay);
        }
    
    if (digitalRead(g2Switch) == LOW){
        analogWrite(gLED, 0);
    }
        else{
            analogWrite(gLED, brightness);
            delay(ledDelay);
            analogWrite(gLED,0);
            delay(ledDelay);
        }
    
    if (digitalRead(g3Switch) == LOW){
        analogWrite(bLED, 0);
    }
        else{
            analogWrite(bLED, brightness);
            delay(ledDelay);
            analogWrite(bLED,0);
            delay(ledDelay);
        }

    delay(1000);  // Delay between the next round of flashes
        
} // END LOOP



    // This mess of code is the function that gets called from SmartThings (or Particle or Google or Alexa) to trigger the relays.
    // It's outside of the loop, so it only runs when it's called.
    int GarageRelay(String command){
        
        if (command == "1") {  // Toggle g1Relay with a single LED flash in the middle for feedback and delay adjusted for a 1 second toggle
            digitalWrite(g1Relay, HIGH);
            delay(250);
            digitalWrite(triggerLED, HIGH);
            delay(100);
            digitalWrite(triggerLED, LOW);
            delay(650);
            digitalWrite(g1Relay, LOW);
            Particle.publish("GarageRelay", "Garage 1 Relay Toggled!", PRIVATE);
	        return 1;
        } 
        
        else if (command == "2"){  // Toggle g2Relay with a double LED flash in the middle for feedback and delay adjusted for a 1 second toggle
            digitalWrite(g2Relay, HIGH);
            digitalWrite(triggerLED, HIGH);
            delay(250);
            delay(100);
            digitalWrite(triggerLED, LOW);
            delay(100);
            digitalWrite(triggerLED, HIGH);
            delay(100);
            digitalWrite(triggerLED, LOW);
            delay(450);
            digitalWrite(g2Relay, LOW);
            Particle.publish("GarageRelay", "Garage 2 Relay Toggled!", PRIVATE);
            return 2;
        } 
        
        else if (command == "3"){  // Toggle g3Relay with a triple LED flash in the middle for feedback and delay adjusted for a 1 second toggle
            digitalWrite(g3Relay, HIGH);
            delay(250);
            digitalWrite(triggerLED, HIGH);
            delay(100);
            digitalWrite(triggerLED, LOW);
            delay(100);
            digitalWrite(triggerLED, HIGH);
            delay(100);
            digitalWrite(triggerLED, LOW);
            delay(100);
            digitalWrite(triggerLED, HIGH);
            delay(100);
            digitalWrite(triggerLED, LOW);
            delay(250);
            digitalWrite(g3Relay, LOW);
            Particle.publish("GarageRelay", "Garage 3 Relay Toggled!", PRIVATE);
	        return 3;
        }
        
        else if (command == "open1"){  // open garage 1 only if it's closed
            if (g1Status == "Closed"){
                GarageRelay("1");
            }
            else Particle.publish("GarageRelay", "Garage 1 requested open but is already open.", PRIVATE);
            }
            
        else if (command == "close1"){  // close garage 1 only if it's open
            if (g1Status == "Open"){
                GarageRelay("1");
            }
            else Particle.publish("GarageRelay", "Garage 1 requested closed but is already closed.", PRIVATE);
            }
        
        else if (command == "open2"){  // open garage 2 only if it's closed
            if (g2Status == "Closed"){
                GarageRelay("2");
            }
            else Particle.publish("GarageRelay", "Garage 2 requested open but is already open.", PRIVATE);
            }
            
        else if (command == "close2"){  // close garage 2 only if it's open
            if (g2Status == "Open"){
                GarageRelay("2");
            }
            else Particle.publish("GarageRelay", "Garage 2 requested closed but is already closed.", PRIVATE);
            }
        
        else if (command == "open3"){  // open garage 3 only if it's closed
            if (g3Status == "Closed"){
                GarageRelay("3");
            }
            else Particle.publish("GarageRelay", "Garage 3 requested open but is already open.", PRIVATE);
            }
            
        else if (command == "close3"){  // close garage 3 only if it's open
            if (g3Status == "Open"){
                GarageRelay("3");
            }
            else Particle.publish("GarageRelay", "Garage 3 requested closed but is already closed.", PRIVATE);
            }
        
        else if (command == "openAll"){  // trigger all the garage doors only if they're closed
            GarageRelay("open1");
            GarageRelay("open2");
            GarageRelay("open3");
        }
        
        else if (command == "closeAll"){ // trigger all garage doors only if they're open
            GarageRelay("close1");
            GarageRelay("close2");
            GarageRelay("close3");
        }
    }
