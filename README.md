## RCSwitchMqttGate
Remote Control and MQTT Gateway of Livolo Switch & Other (RCSwitch library supported devices) 433Mhz devices
##
MQTT Gateway based on Expressif microcontroller ESP8266 oriented to the RF control of Livolo switches and also those supported by the own RCSwitch librarywith any of the typical OOK/ASK transmitter modules of 433.92 Mhz like STX882, FS1000A, HPD8407-F, etc ...) or many transceivers than can work on OOK/ASK (and have their propper ESP8266 library) like RFM69.
##
It makes use of internal autogeneration of remote id livolo codes (tested working with a total of +30 switch devices) with the support of the additional use of scene codes I and II (code 10 and code 18 for room 1) that allows us the management using separately ON and OFF commands for thus avoid using the only toggle ON / OFF switch command that leave the device in a unknow state because their lack of feedback.
##
The livolo switch "pairing" is done very easily by placing the switch in learn/pairing mode (by pressing it for 5 seconds) and sending to the device the On or the Off command corresponding to the desired device state.
##
The form of the MQTT command topic / subtopic and command is as easy as follows.

(main topic path).../livolo/switch (1 ... 30) / (0 or 1).

Example for switching on switch 1.

(main topic path).../livolo/switch1/1.

Topic: livolo.
Subtopic: switch1 (01 to 99).
Command: 0 (OFF) or 1 (ON).


