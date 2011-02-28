// Aditional Librarys which make things easier
#include <WString.h>
// I had to modify the core files for Sheild for this to work
// i.e making HardwareSerial extend from Print or better yet
// SerialBase
#include <Streaming.h>

// the CmdMessenger library and stuff
#include <CmdMessenger.h>

// Cmds handled by the arduino
// They will be called from TouchShield
#define BUTPRESS_ARDUINO_CMD 1
#define MESSAGE_ARDUINO_CMD 2

// Cmds handled by the TouchShield
// They will be called from Arduino
#define DRAWELLIPSE_TOUCH_CMD 1
#define MESSAGE_TOUCH_CMD 2

CmdMessenger cmdMessenger = CmdMessenger(Serial);

// ------------------ C A AL L  B A C K  M E T H O D S ------------------
// Default handler for unknown CmdMessages
void unknownCmd()
{
  // Print message to screen
  stroke(40,40,255);
  text("Unknown Message", 10, 10, 100, 10);
}

void drawSmallEllipse()
{
  ellipse(140, 140, 15, 20);
}

void drawEllipseCmd()
{
 // while ( cmdMessenger.available() ) {    
    if ( cmdMessenger.checkString("green") )
      fill(50, 255, 50); // fill following shapes with a red
    else if ( cmdMessenger.checkString("red") )
      fill(255, 60, 50); // fill following shapes with a red   

    // could get other arguments or paramters for drawing here
//  }
  drawSmallEllipse();
}

void messageCmd()
{
  char buf[50];
  String str = String(10);
  boolean reqAcn = false;

  if ( cmdMessenger.checkString("A") )
    reqAcn = true;

  if ( cmdMessenger.checkString("debug") )
    str = "DEBUG::";
  else if ( cmdMessenger.checkString("error") )
    str = "DEBUG::";
  else if ( cmdMessenger.checkString("warming") )
    str = "WARNING::";  

  while ( cmdMessenger.available() ) {    
    cmdMessenger.copyString(buf, 50);    
    str.append(buf);
  }
  
  // Clear the area that text gets drawn to  
  fill(0);
  stroke(0);
  rect(0, 0, 200, 20);
  // Show on screen the counter from Arduino
  stroke(40,40,255);
  text(str, 10, 10, 100, 10);
  
  // just debug and let Arduion know we got something
  cmdMessenger.sendCmd(MESSAGE_ARDUINO_CMD, "touchSheild here, got new count thanks");
}

// ------------------ E N D  C A AL L  B A C K  M E T H O D S ------------------

void setup()
{  
  Serial.begin(115200);//9600); // begin serial

  // Attach the CmdMessenger handler functions
  cmdMessenger.attach(unknownCmd);
  cmdMessenger.attach(DRAWELLIPSE_TOUCH_CMD, drawEllipseCmd);
  cmdMessenger.attach(MESSAGE_TOUCH_CMD, messageCmd);

  background(0,0,0); // turn all the pixels black
  fill(255,50,50); // fill following shapes with a red
  stroke(0,255,100); // outline following shapes with a green color
  ellipse(80,80,30,30); // draw an ellipse around point (80, 80) and make it 30 x 30 (long x wide)

  drawSmallEllipse();
}

void loop()
{
  // All in an IF so we only do this when a new touch even occures
  if ( gettouch()) { // sample the touch screen

    // did someone touch our circle ?
    // Touching circle sends a message to Arduino, and requires an Acknowledgement
    // touching outside circle also sends message but no acknowledgement
    if(mouseX > 50 & mouseX < 110 & mouseY > 50 & mouseY < 110)  
    {
      fill(50, 255,50); // fill following shapes with a red
      ellipse(80,80,30,30);
      cmdMessenger.sendCmd(BUTPRESS_ARDUINO_CMD, "A");
      // wait for acknowledgement... want to do this nicer in the sendCmd
      while ( !Serial.available() ) ;
      char res = Serial.read();
      // Just send another message to let Arduino know we got the message
      // could go on like this all day.
      cmdMessenger.sendCmd(MESSAGE_ARDUINO_CMD, "I got the acknoledge thanks");
      delay(200); // delay for 10 seconds  
    } else
    {
      fill(255,50,50); // fill following shapes with a red
      stroke(0,255,100); // outline following shapes with a green color    
      ellipse(80,80,30,30);
      cmdMessenger.sendCmd(BUTPRESS_ARDUINO_CMD, "B");
      delay(200); // delay for 10 seconds  
    }    
  }

  // Process the serial comming in on Serial from Arduino
  cmdMessenger.feedinSerialData();

  // Just echo back any commands - for testing commands entered via the pc
  //while (Serial.available() ) Serial.print(Serial.read());
}
