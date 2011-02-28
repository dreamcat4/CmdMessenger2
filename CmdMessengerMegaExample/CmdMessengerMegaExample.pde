// This example demonstrates CmdMessenger's callback  & attach methods

// To make printing to Serial nicer to type
#include <Streaming.h>
#include <WString.h>

// Cmds handled by the arduino
// They will be called from TouchShield
#define BUTPRESS_ARDUINO_CMD 1
#define MESSAGE_ARDUINO_CMD 2


// Cmds handled by the TouchShield
// They will be called from Arduino
#define DRAWELLIPSE_TOUCH_CMD 1
#define MESSAGE_TOUCH_CMD 2

#include <CmdMessenger.h>
// Instantiate Messenger object with the message function and 
// the default separator (the space character) connect the hardware
// Serial1 (TouchShieldSlide, or some other thing)
CmdMessenger cmdMessenger = CmdMessenger(Serial1); 

// ------------------ C A AL L  B A C K  M E T H O D S ------------------

// A button has been pressed CmdMessenger function
void buttPressCmd() 
{
  // Loop through all the available elements of the message
  while ( cmdMessenger.available() ) {
    char state = cmdMessenger.readChar();
    // Set the pin as determined by the message
    Serial << "Button Pressed " << state << endl;
  }
}


// Default handler for unknown CmdMessages
void unknownCmd()
{
 Serial << "UNknown command" << endl; 
}

// Take a message and relay it to the Serial PC console 
void messageCmd() 
{
  char buf[350];
  boolean reqAcn = false;
  
  if ( cmdMessenger.checkString("A") )
    reqAcn = true;
  
  if ( cmdMessenger.checkString("debug") )
    Serial << "DEBUG::";
  else if ( cmdMessenger.checkString("error") )
    Serial << "DEBUG::";
  else if ( cmdMessenger.checkString("warming") )
    Serial << "WARNING::";  
  
  while ( cmdMessenger.available() ) {    
    cmdMessenger.copyString(buf, 350);
    Serial << buf << " ";
  }
  Serial << endl;
  
  if (reqAcn) Serial1 << "msg recieved thanks" << endl;
}

// ------------------ E N D  C A AL L  B A C K  M E T H O D S ------------------

void setup() 
{
  // Initiate Serial Communication
  Serial1.begin(115200);//9600);  // To TouchShield
  Serial.begin(115200);//9600);   // To Pc Terminal
  
  // Attach the CmdMessenger handler functions
  cmdMessenger.attach(unknownCmd);
  cmdMessenger.attach(BUTPRESS_ARDUINO_CMD, buttPressCmd);
  cmdMessenger.attach(MESSAGE_ARDUINO_CMD, messageCmd);
  
  Serial << "Starting up CmdMessenger Example" << endl;
}

// For flashing ellipse on Touch
long interval = 5000; // 2 seconds
long previousMillis = 0;
boolean red = true;
int counter = 0;
String str = String(20);
  
void loop() 
{
  // Process the serial comming in on Serial1 from TouchShield 
  cmdMessenger.feedinSerialData();

  // every interval send command to TouchShield to change 
  // color of small ellipse
  if (  millis() - previousMillis > interval ) {
    if (red) {
      cmdMessenger.sendCmd(DRAWELLIPSE_TOUCH_CMD, "red");
      red = false;
    } else {
      cmdMessenger.sendCmd(DRAWELLIPSE_TOUCH_CMD, "green");
      red = true;
    }
    previousMillis = millis();
    counter ++;

    str = "Counter= ";
    str.append(counter);
    cmdMessenger.sendCmd(MESSAGE_TOUCH_CMD, str);
  }
  
  while ( Serial.available( ) )    Serial1.print( Serial.read( ) );
}