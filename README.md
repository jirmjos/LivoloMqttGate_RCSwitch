## RCSwitchMqttGate
Remote Control and MQTT Gateway of Livolo Switch & Other (RCSwitch library supported devices) 433Mhz devices
##
MQTT Gateway based on Expressif microcontroller ESP8266 oriented to the RF control of Livolo switches and also those supported by the own RCSwitch librarywith any of the typical OOK/ASK transmitter modules of 433.92 Mhz like STX882, FS1000A, HPD8407-F, etc ...) or many transceivers than can work on OOK/ASK (and have their propper ESP8266 library) like RFM69.
##
It makes use of internal autogeneration of remote id livolo codes (tested working with all type of switch devices) with the support of the additional use of scene codes I and II (code 10, 18, 90, 114...) that allows us the management using separately ON and OFF commands for thus avoid using the only toggle ON / OFF switch command that leave the device in a unknow state because their lack of feedback.
##
All switch type are now supported combining scenes to push it to on and "standard" all off command (two ways switch are limited to learn only one scene code id).
Switch numbers from 95 to 99 (both included) are reserved for dimmer management because in this case no "standard" all off code command are supported.
Actually I have limited on code a total range switch management to 100 devices (00 to 99) that I think is more than enough to majority of uses, but of course this range can be extended with tribial code changes.
##
The livolo switch "pairing" now is done very easily by placing the switch in learn/pairing mode (by pressing it for 5 seconds) and send to the livolo device the On or the Off command corresponding to the desired device state.
You need do the pairing two times one corresponding to on state and once more to the off state.
##

The generic form of the MQTT command topic / subtopic and command is as easy as follows:
(main topic path).../livolo/switch(1 ... 99) / (0 or 1).

Example for command to switching on switch=01:
(main topic path).../livolo/switch01
Payload: 1

Subtopic ... /livolo/switch(01 to 99)
Command: 0 (OFF) or 1 (ON)

NOTE: Original idea and source code from https://github.com/SergeyKrutyko/RCSwitchMqttGate
