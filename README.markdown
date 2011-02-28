### CmdMessenger - Version 2.0

For the Arduino Platform.

### Intoduction

CmdMessenger is a command-orientated version of Messenger. To use CmdMessenger, we define a list of command identifiers. And attach callback / handler functions for recieved messages. The first sub-string (part) in a message is always the command identifier. If a message is received and there is no corresponding known command (and command handler function), then the message is passed to the default message handler.

This new version of the library (version 2.0) contains many changes to both the CmdMessenger class and also the example program.

### Requirements

* Arduino IDE Version 022 or later.

  Earlier versions eg 021 may work but are not tested.

* Ardunio Streaming Library

  [http://arduiniana.org/libraries/streaming/](http://arduiniana.org/libraries/streaming/)

* Arduino Base64 Library

  [https://github.com/adamvr/arduino-base64](https://github.com/adamvr/arduino-base64)


### Getting Started

Open CmdMessengerExample.pde and upload it to your Arduino. Check the baud rate is OK for your model. Everything is explained as comments within the sktech file.

Dreamcat4

### Links

[http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1222259218/all](http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1222259218/all)

[http://todbot.com/blog/2006/12/06/arduino-serial-c-code-to-talk-to-arduino](http://todbot.com/blog/2006/12/06/arduino-serial-c-code-to-talk-to-arduino)



### Credit

* Messenger Library     - By Thomas Ouellet Fredericks.
* CmdMessenger Verion 1 - By Neil Dudman.
* CmdMessenger Verion 2 - By Dreamcat4.

