### CmdMessenger - Version 2.0

For the Arduino Platform.

### Intoduction

CmdMessenger is a command-orientated version of Messenger. To use CmdMessenger, we define a list of command identifiers. And attach callback / handler functions for recieved messages. The first sub-string (part) in a message is always the command identifier. If a message is received and there is no corresponding known command (and command handler function), then the message is passed to the default message handler.

This new version of the library (version 2.0) contains many changes to both the CmdMessenger class and also the example program.

### Improvements Over CmdMessenger v1 (v0.1.0)

* Updated to work with Arduino IDE 022
* Enable / disable newline (print and ignore)
* New generic example (works with all Arduinos)
* More reliable process() loop.
* User can set their own cmd and field seperator
 (defaults to ';' and ',')
* Base-64 encoded data to avoid collisions with ^^
* Works with Arduino Serial Monitor for easy debugging

### Requirements

* [Arduino IDE Version 022 or later](http://www.arduino.cc/en/Main/Software)* 
* [Ardunio Streaming Library](http://arduiniana.org/libraries/streaming/)
* [Arduino Base64 Library](https://github.com/adamvr/arduino-base64)

\* Earlier versions of Arduino IDE might work but wernt tested.

### Getting Started

Open CmdMessengerExample.pde and upload it to your Arduino. Check the baud rate is OK for your model. Everything is explained as comments within the sktech file.

Dreamcat4

### Links

[http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1222259218/all](http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1222259218/all)

[http://todbot.com/blog/2006/12/06/arduino-serial-c-code-to-talk-to-arduino](http://todbot.com/blog/2006/12/06/arduino-serial-c-code-to-talk-to-arduino)



### Credit

* Initial Messenger Library - By Thomas Ouellet Fredericks.
* CmdMessenger Version 1 - By Neil Dudman.
* CmdMessenger Version 2 - By Dreamcat4.

