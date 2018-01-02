/**
 *  GarageRelay
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
} 
 
metadata {
	definition (name: "GarageRelay", namespace: "jshearer", author: "Jared Shearer") {
		capability "Alarm"
		capability "Contact Sensor"
        capability "Sensor"
		capability "Polling"
		capability "Refresh"
		capability "Switch"
		capability "Temperature Measurement"

	/*	//commented out and replaced with below code
        attribute "switchUnLock", "string"
		attribute "switchLock", "string"
		attribute "switchTrunk", "string"
		attribute "switchStart", "string"
		attribute "switchPanic", "string"
		command "cmdUnLock"
		command "cmdLock"
		command "cmdTrunk"
		command "cmdStart"
		command "cmdPanic"
	*/
    
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
		        
			standardTile("garage1", "device.garage1", width: 2, height: 2, inactiveLabel: false, decoration: "flat") {
				state "on", label: "Left", action: "garage1", icon: "st.shields.shields.arduino", backgroundColor: "#79b821"
				state "off", label: "Left", action: "garage1", icon: "st.shields.shields.arduino", backgroundColor: "#ffffff"
			}
    		standardTile("garage2", "device.garage2", width: 2, height: 2, inactiveLabel: false, decoration: "flat") {
				state "on", label: "Middle", action: "garage2", icon: "st.shields.shields.arduino", backgroundColor: "#79b821"
				state "off", label: "Middle", action: "garage2", icon: "st.shields.shields.arduino", backgroundColor: "#ffffff"
			}
    		standardTile("garage3", "device.garage3", width: 2, height: 2, inactiveLabel: false, decoration: "flat") {
				state "on", label: "Right", action: "garage3", icon: "st.shields.shields.arduino", backgroundColor: "#79b821"
				state "off", label: "Right", action:"garage3", icon: "st.shields.shields.arduino", backgroundColor: "#ffffff"
			}         
            standardTile("contact1", "device.contact1", width: 2, height: 2) {
				state "Open", label: "OPEN", icon: "st.doors.garage.garage-open", backgroundColor: "##79b821"
				state "Closed", label: "CLOSED", icon: "st.doors.garage.garage-closed", backgroundColor: "#e86d13"
			}
    		standardTile("contact2", "device.contact2", width: 2, height: 2) {
				state "Open", label: "OPEN", icon: "st.doors.garage.garage-open", backgroundColor: "##79b821"
				state "Closed", label: "CLOSED", icon: "st.doors.garage.garage-closed", backgroundColor: "#e86d13"
		    }
            standardTile("contact3", "device.contact3", width: 2, height: 2) {
				state "Open", label: "OPEN", icon: "st.doors.garage.garage-open", backgroundColor: "##79b821"
				state "Closed", label: "CLOSED", icon: "st.doors.garage.garage-closed", backgroundColor: "#e86d13"
		    }
            
            
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
	main (["garage3"])
    details(["contact1","contact2","contact3","garage1","garage2","garage3","temperature","refresh"])
	}
}

// parse events into attributes
def parse(String description) {
	log.debug "Parsing '${description}'"
	// TODO: handle 'alarm' attribute
	// TODO: handle 'battery' attribute
	// TODO: handle 'switch' attribute
	// TODO: handle 'temperature' attribute
	// TODO: handle 'switchUnLock' attribute
	// TODO: handle 'switchLock' attribute
	// TODO: handle 'switchTrunk' attribute
	// TODO: handle 'switchStart' attribute
	// TODO: handle 'switchPanic' attribute

}

// handle commands


def strobe() {
	log.debug "Executing 'strobe'"
	// TODO: handle 'strobe' command
}

def siren() {
	log.debug "Executing 'siren'"
	// TODO: handle 'siren' command
}

def both() {
	log.debug "Executing 'both'"
	// TODO: handle 'both' command
}

def poll() {
	log.debug "Executing 'poll'"
	// TODO: handle 'poll' command
}

def refresh() {
	log.debug "Executing 'refresh'"
	// TODO: handle 'refresh' command
}

def on() {
	log.debug "Executing 'on'"
	// TODO: handle 'on' command
}

def off() {
	log.debug "Executing 'off'"
	// TODO: handle 'off' command
}

 // 1 = Unlock		switchUnLock	cmdUnLock
 // 2 = Lock		switchLock		cmdLock
 // 3 = Trunk		switchTrunk		cmdTrunk
 // 4 = Start		switchStart		cmdStart
 // 5 = Panic		switchPanic		cmdPanic

def garage1() {
	log.debug "Executing 'garage1'"
	// TODO: handle 'cmdUnLock' command
    put '1'
}

def garage2() {
	log.debug "Executing 'garage2'"
	// TODO: handle 'cmdLock' command
    put '2'
}

def garage3() {
	log.debug "Executing 'garage3'"
	// TODO: handle 'cmdTrunk' command
    put '3'
}

/* COMMENTING OUT EXTRA COMMANDS
def cmdStart() {
	log.debug "Executing 'cmdStart'"
	// TODO: handle 'cmdStart' command
    put '4'
}
def cmdPanic() {
	log.debug "Executing 'cmdPanic'"
	// TODO: handle 'cmdPanic' command
    put '5'
}
*/

private put(ValveAction) {
	log.debug "sending post";
		httpPost(
			uri: "https://api.spark.io/v1/devices/${deviceId}/GarageRelay",
	        body: [access_token: token, command: ValveAction],  
		) {response -> log.debug (response.data)}
	log.debug "post sent";
    }
	
	def GetValveState() {
		def params = [
		uri: "https://api.spark.io/v1/devices/${deviceId}/GarageState?access_token=${token}"]		
		try{
			httpGet(params){ resp ->
                    return resp.data.result;
			    }
			} catch (e) {
			    log.error "something went wrong: $e"
			}
	}
