/**
 *  GarageControl
 *
 *  Copyright 2017 Jared Shearer
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 *  in compliance with the License. You may obtain a copy of the License at:
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software distributed under the License is distributed
 *  on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License
 *  for the specific language governing permissions and limitations under the License.
 *
 */

 
preferences {
    input("deviceId", "text", title: "Device ID")
    input("token", "text", title: "Access Token")

	// This is where we define the names of the "switches" on the Particle that are sensing the state of the doors
	input name: "g1State", type: "text", title: "Garage 1 Switch", required: true, defaultValue: "g1State"
    input name: "g2State", type: "text", title: "Garage 2 Switch", required: true, defaultValue: "g2State"
    input name: "g3State", type: "text", title: "Garage 3 Switch", required: true, defaultValue: "g3State"
} 
 
metadata {
	definition (name: "GarageControl", namespace: "jshearer", author: "Jared Shearer") {
		capability "Contact Sensor"
        capability "Sensor"
		capability "Polling"
		capability "Refresh"
		capability "Switch"
		capability "Temperature Measurement"
        attribute "g1State", "number"
        attribute "g2State", "number"
        attribute "g3State", "number"

        attribute "garage1", "string"
		attribute "garage2", "string"
		attribute "garage3", "string"
		
		command "garage1"
		command "garage2"
		command "garage3"
		
    }

	simulator {
		// TODO: define status and reply messages here
	}

	tiles(scale: 2) {
		// TODO: define your main and details tiles here
		    
            valueTile("g1State", "device.g1State", width: 2, height: 1) {
            state("0", label:'${currentValue}')
            state("1", label:'${currentValue}')
        	}
            valueTile("g2State", "device.g2State", width: 2, height: 1) {
            state("g2State", label:'${currentValue}')
        	}
            valueTile("g3State", "device.g3State", width: 2, height: 1) {
            state("g3State", label:'${currentValue}')
        	}
            
            /* COMMENTED OUT BECAUSE THAT DIDN'T WORK...
            standardTile("contact1", "device.g1State", width: 2, height: 2) {
				state "0", label: "OPEN", icon: "st.doors.garage.garage-open", backgroundColor: "##79b821"
				state "1", label: "CLOSED", icon: "st.doors.garage.garage-closed", backgroundColor: "#e86d13"
		    }
    		standardTile("contact2", "device.g2State", width: 2, height: 2) {
				state "0", label: "OPEN", icon: "st.doors.garage.garage-open", backgroundColor: "##79b821"
				state "1", label: "CLOSED", icon: "st.doors.garage.garage-closed", backgroundColor: "#e86d13"
		    }
            standardTile("contact3", "device.g3State", width: 2, height: 2) {
				state "0", label: "OPEN", icon: "st.doors.garage.garage-open", backgroundColor: "##79b821"
				state "1", label: "CLOSED", icon: "st.doors.garage.garage-closed", backgroundColor: "#e86d13"
		    }
            */
            
            standardTile("garage1", "device.garage1", width: 2, height: 2, inactiveLabel: false, decoration: "flat") {
				state "on", label: "Left", action: "garage1", icon: "st.shields.shields.arduino", backgroundColor: "#ffffff"
				state "off", label: "Left", action: "garage1", icon: "st.shields.shields.arduino", backgroundColor: "#ffffff"
			}
    		standardTile("garage2", "device.garage2", width: 2, height: 2, inactiveLabel: false, decoration: "flat") {
				state "on", label: "Middle", action: "garage2", icon: "st.shields.shields.arduino", backgroundColor: "#ffffff"
				state "off", label: "Middle", action: "garage2", icon: "st.shields.shields.arduino", backgroundColor: "#ffffff"
			}
    		standardTile("garage3", "device.garage3", width: 2, height: 2, inactiveLabel: false, decoration: "flat") {
				state "on", label: "Right", action: "garage3", icon: "st.shields.shields.arduino", backgroundColor: "#ffffff"
				state "off", label: "Right", action:"garage3", icon: "st.shields.shields.arduino", backgroundColor: "#ffffff"
			}         

            
            
			// TO DO
            valueTile("temperature", "device.temperature", width: 2, height: 2) {
				state("temperature", label:'${currentValue}°', unit:"F",
				backgroundColors:[
					[value: 31, color: "#153591"],
					[value: 44, color: "#1e9cbb"],
					[value: 59, color: "#90d2a7"],
					[value: 74, color: "#44b621"],
					[value: 84, color: "#f1d801"],
					[value: 95, color: "#d04e00"],
					[value: 96, color: "#bc2323"]
				]
			)
			}
			standardTile("refresh", "device.switch", width: 2, height: 2, inactiveLabel: false, decoration: "flat") {
		    	state "default", label:'', action:"refresh.refresh", icon:"st.secondary.refresh"
			}
	main (["g3State"])
    details(["g1State","g2State","g3State",/*"contact1","contact2","contact3",*/"garage1","garage2","garage3","temperature","refresh"])
	}
}

// I have no idea if I need this
def parse(String description) {
    def pair = description.split(":")

    createEvent(name: pair[0].trim(), value: pair[1].trim())
}

// handle commands

def garage1() {
	log.debug "Executing 'garage1'"
	put '1'
    refresh()
}

def garage2() {
	log.debug "Executing 'garage2'"
	put '2'
    refresh()
}

def garage3() {
	log.debug "Executing 'garage3'"
	put '3'
    refresh()
}

def poll() {
    log.debug "Executing 'poll'"
    refresh()
}    

def refresh() {
	log.debug "Executing 'refresh'"
    getg1State()
    getg2State()
    getg3State()
}

// How we push commands from the device handler to the Particle
private put(GarageRelay) {
	log.debug "sending post";
		httpPost(
			uri: "https://api.particle.io/v1/devices/${deviceId}/GarageRelay",
	        body: [access_token: token, command: GarageRelay],  
		) {response -> log.debug (response.data)}
	log.debug "post sent";
    }

// These three are how we are (theoretically) getting the state of the doors from the Particle
private getg1State() {
    def closure = { response ->
        log.debug "g1State request was successful, $response.data"

        sendEvent(name: "g1State", value: response.data.result)
    }

    httpGet("https://api.particle.io/v1/devices/${deviceId}/${g1State}?access_token=${token}", closure)
	}
    
private getg2State() {
    def closure = { response ->
        log.debug "g2State request was successful, $response.data"

        sendEvent(name: "g2State", value: response.data.result)
    }

    httpGet("https://api.particle.io/v1/devices/${deviceId}/${g2State}?access_token=${token}", closure)
	}

private getg3State() {
    def closure = { response ->
        log.debug "g3State request was successful, $response.data"

        sendEvent(name: "g3State", value: response.data.result)
    }

    httpGet("https://api.particle.io/v1/devices/${deviceId}/${g3State}?access_token=${token}", closure)
	}
