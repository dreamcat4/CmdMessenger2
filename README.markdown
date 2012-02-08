### CmdMessenger v2

A Serial Messaging system for the Arduino Platform.

### Intoduction

CmdMessenger is a command-orientated version of Messenger for serial comms. To use CmdMessenger, we define a list of command identifiers. And attach callback / handler functions for recieved messages. The first sub-string (part) in a message is always the command identifier. If a message is received and there is no corresponding known command (and command handler function), then the message is passed to the default message handler.

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

Pretty much everything is explained within the example sketch file. Open CmdMessengerExample.pde in Arduino IDE and upload it to your microcontroller device. Check the baud rate is OK for your model. Try it out by typing commands into the Arduino Serial Monitor.

Once its been established what is happening on the Arduino side by following the interactive example, the next step is to work on with the program running on the other side of the serial interface which will be communicating with the Arduino. There are a wide variety of platforms and programming languages which can be used. C, Python, Ruby, Linux, Windows, and other embedded devices. The possibilities are endless. A principal benefit of CmdMessenger is that the format of messages are both flexible and simple enough to be programmed in nearly any suitable language or programming environment that supports a serial (comm port) interface.

The following C program `arduino-serial` is recommended for this task. It can be modified to suit your needs, or compiled "as-is" to provide an appropriate command line tool for scripting.

* [arduino-serial.c](http://todbot.com/blog/2006/12/06/arduino-serial-c-code-to-talk-to-arduino)

NOTE: Don't put `arduino-serial.c` into your `Arduino/libraries/CmdMessenger/` directory or CmdMessenger won't compile anymore.

We provide a Max5 / MaxMSP example (`/Max5` folder) because historically that has been included with previous distributions of Messenger. The inclusion of a MaxMSP sample is not any kind of recommendation over other languages however.

Dreamcat4


### Credit

* Initial Messenger Library - By Thomas Ouellet Fredericks.
* CmdMessenger Version 1 - By Neil Dudman.
* CmdMessenger Version 2 - By Dreamcat4.

