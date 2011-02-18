### CmdMessenger

For the Arduino Platform. By Neil Dudman.

### Intoduction

I've added a few things in my own class CmdMessenger which extend from Messenger. Had to make a few small changes to the origional Messenger which I've included here along with my CmdMessenger class if its useful to anyone else.

Basicly with CmdMessenger sub class you can attach more than 1 messenger handler, which get called according to the first string (cmd) in a message. If a valid message for the command hasn't been attached then the single default message handler of Messenger is called. 

[http://www.arduino.cc/playground/uploads/Code/CmdMessenger.zip](http://www.arduino.cc/playground/uploads/Code/CmdMessenger.zip)

The zip file contains the orgional Messenger with my small changes, the new CmdMessenger class and two sample programs (one runs on Arduino the other on a TouchShieldSlide) connected via hardware Serial1. If your using softwareSerial, which dosn't have the Serial.available() method then you will need to change the feedinSerialData() mehod.



Neil

### Links

[http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1222259218/all](http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1222259218/all)

[http://todbot.com/blog/2006/12/06/arduino-serial-c-code-to-talk-to-arduino](http://todbot.com/blog/2006/12/06/arduino-serial-c-code-to-talk-to-arduino)

