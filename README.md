## RCSwitchMqttGate
Remote Control and MQTT Gateway of Livolo Switch & Other (RCSwitch library supported devices) 433Mhz devices
##
MQTT Gateway based on Expressif microcontroller ESP8266 oriented to the RF control of Livolo switches and also those supported by the own RCSwitch librarywith any of the typical OOK/ASK transmitter modules of 433.92 Mhz like STX882, FS1000A, HPD8407-F, etc ...) or many transceivers than can work on OOK/ASK (and have their propper ESP8266 library) like RFM69.
##
It makes use of internal autogeneration of remote id livolo codes (tested working with all type of switch devices) with the support of the additional use of scene codes I and II (code 10, 18, 90, 114...) that allows us the management using separately ON and OFF commands for thus avoid using the only toggle ON / OFF switch command that leave the device in a unknow state because their lack of feedback.
##
All switch type are now supported combining scenes to push it to on and "standard" all off command (livolo two ways switch are limited to learn only one scene code id).
Actually I have limited on code a total range switch management to 100 devices (00 to 99) that I think is more than enough to majority of uses, but of course this range can be extended with tribial code changes.
Switch numbers from 00 to 94 are for switch devices and 95 to 99 (both included) are reserved for dimmer management because in this case no "standard" all off code command are supported.
##
The livolo switch "pairing" now is done very easily by placing the switch in learn/pairing mode (by pressing it for 5 seconds until hear one beep) and send to the livolo device the On and Off command. Of course , you need do the pairing two times one corresponding to On state and repeat pairing process for the Off state.
NOTE: Making the On pairing process you only need take care the switch device state remains On (red light). This is default state when you put the device switch to pairing, but better you knows that.
##

The generic form of the MQTT command topic / subtopic and command is as easy as follows:
(main topic path).../livolo/switch(00 ... 99)  and Payload 0 (off) or 1(on).

Example for command to switching on switch=01:
(main topic path).../livolo/switch01
Payload: 1 (ON)
OTE: You need to use two digits on switch number because my quick dirty way of parsing mqtt command, but I thing too is better when reading mqqt command.

NOTE: Original idea and source code from https://github.com/SergeyKrutyko/RCSwitchMqttGate
