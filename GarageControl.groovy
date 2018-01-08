/*
 *  GarageControl
 *
 *  Copyright 2018 by jsheahawk
 *
 *  This SmartThings device handler integrates a Particle (Photon in my case) with a Relay Shield with SmartThings
 *	to open and monitor the state of garage doors.
 *
 *  TO DO: Webhooks to update the garage door status on a status change.  It's currently JFM to me!
 *
 */

preferences {
    input name: "deviceId", type: "text", title: "Device ID", required: true       // Photon device ID (get from build.particle.io)
    input name: "token", type: "password", title: "Access Token", required: true   // Photon super secret access token (get from build.particle.io)
    input name: "g1Status", type: "text", title: "Garage 1 Status Variable Name", required: true, defaultValue: "g1Status"  // These three lines define the name of the variable
    input name: "g2Status", type: "text", title: "Garage 2 Status Variable Name", required: true, defaultValue: "g2Status"  // that you're looking for from your Particle
    input name: "g3Status", type: "text", title: "Garage 3 Status Variable Name", required: true, defaultValue: "g3Status"  // sketch.
    }

metadata {
    definition (name: "GarageControl", namespace: "jshearer", author: "Jared Shearer") {
        capability "Polling"
        capability "Sensor"
        capability "Refresh"
        
        attribute "g1Status", "string"  // Defines a container to hold the state of your garage door
        attribute "g2Status", "string"
        attribute "g3Status", "string"
        
        command "garage1"  // Defines a command to trigger the garage door we want opened
		command "garage2"
		command "garage3"
    }

    tiles(scale: 2) {
    	// Defines tiles that will appear on the device page
   		standardTile("g1Status", "device.g1Status", width: 6, height: 2, inactiveLabel: false, decoration: "flat") {
				state "Closed", label: "Left", action: "garage1", icon: "st.doors.garage.garage-closed", backgroundColor: "#83ED5B"
				state "Open", label: "Left", action: "garage1", icon: "st.doors.garage.garage-open", backgroundColor: "#FF5733"
		}
    	standardTile("g2Status", "device.g2Status", width: 6, height: 2, inactiveLabel: false, decoration: "flat") {
				state "Closed", label: "Middle", action: "garage2", icon: "st.doors.garage.garage-closed", backgroundColor: "#83ED5B"
				state "Open", label: "Middle", action: "garage2", icon: "st.doors.garage.garage-open", backgroundColor: "#FF5733"
		}
        standardTile("g3Status", "device.g3Status", width: 6, height: 2, inactiveLabel: false, decoration: "flat") {
				state "Closed", label: "Right", action: "garage3", icon: "st.doors.garage.garage-closed", backgroundColor: "#83ED5B"
				state "Open", label: "Right", action: "garage3", icon: "st.doors.garage.garage-open", backgroundColor: "#FF5733"
		}
   
        standardTile("refresh", "device.refresh", inactiveLabel: false, decoration: "flat", width: 6, height: 2) {
            state "default", action:"refresh.refresh", icon:"st.secondary.refresh"
        }

		main (["g3Status"])  // This is the tile that will show up on the device's preview.  It's my right garage door in this case
    }

}


// create a new attribute called garagestatus
def updated() {
	initialize()  // I have no idea what this calls.  It isn't in this DH!
    log.debug "Updated !"
    state.g1Status = 1
    state.g2Status = 1
    state.g3Status = 1
    log.debug "device.g1Status: ${device.g1Status}"
    log.debug "device.g2Status: ${device.g2Status}"
    log.debug "device.g3Status: ${device.g3Status}"
}

def poll() {  // Polls for the status of the garage doors
    log.debug "Executing 'poll'"

    getStatus1()
    getStatus2()
    getStatus3()
}

def refresh() {  // Calls the garage status functions to refresh them upon refresh button press
    log.debug "Executing 'refresh'"

    getStatus1()
    getStatus2()
    getStatus3()
}

def garage1() {  // This is what happens when tile g1Status is pushed.
	log.debug "Executing 'garage1'"
	put '1'  // Calls put(GarageRelay) and sends it a "1" which tells the Particle sketch to run GarageRelay with command == 1
    delay(20000)  // I don't know if this delay then refresh works.  Theoretically, it waits 20s then refreshes the status of the doors
    refresh()
}

def garage2() {  // This is what happens when tile g2Status is pushed.
	log.debug "Executing 'garage2'"
	put '2'
    delay(20000)
    refresh()
}

def garage3() {  // This is what happens when tile g3Status is pushed.
	log.debug "Executing 'garage3'"
	put '3'
    delay(20000)
    refresh()
}

// no clue what this does or if its needed
def parse(String description) {
    def pair = description.split(":")

    createEvent(name: pair[0].trim(), value: pair[1].trim())
}

// ask smartthings for the garage status
private getStatus1() {
    def closure = { response ->
        log.debug "request was successful, $response.data"

        sendEvent(name: "g1Status", value: response.data.result)
    }

    httpGet("https://api.particle.io/v1/devices/${deviceId}/${g1Status}?access_token=${token}", closure)
}

private getStatus2() {
    def closure = { response ->
        log.debug "request was successful, $response.data"

        sendEvent(name: "g2Status", value: response.data.result)
    }

    httpGet("https://api.particle.io/v1/devices/${deviceId}/${g2Status}?access_token=${token}", closure)
}

private getStatus3() {
    def closure = { response ->
        log.debug "request was successful, $response.data"

        sendEvent(name: "g3Status", value: response.data.result)
    }

    httpGet("https://api.particle.io/v1/devices/${deviceId}/${g3Status}?access_token=${token}", closure)
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
