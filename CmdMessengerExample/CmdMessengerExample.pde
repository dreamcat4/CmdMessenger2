// This example demonstrates CmdMessenger's callback  & attach methods
// For Arduino Uno and Arduino Duemilanove board (may work with other)

#include <Streaming.h> // Nicer printing to Serial
#include <CmdMessenger.h>

// Base64 library available from https://github.com/adamvr/arduino-base64
// Download it into your Sketches/libraries/ folder
#include <Base64.h>

// Must not conflict with or other raw message data. Fine if we use base64 library
char field_separator = ',';
char command_separator = ';';

// Instantiate Messenger object with Serial
CmdMessenger cmdMessenger = CmdMessenger(Serial, field_separator, command_separator);


// ------------------ C M D  L I S T I N G ( T X / R X ) ---------------------

// Commands we send from the Arduino to be received on the PC
enum
{
  kCOMM_ERROR    = 000, // Reserved for CmdMessenger
  kACK           = 001, // Acknowledge cmd received
  kARDUINO_READY = 002, // For PC to ping our arduino
  kERR           = 003, // Report bad cmd, cmd error
  // We can define more commands here, eg
  // kICE_CREAM_READY,
  // kICE_CREAM_PRICE,
  // And it will increase our start offset for vv the other cmds below
  kSEND_CMDS_END, // Mustnt delete this line
};

// Commands we send from the PC and recieve in callbacks on the Arduino
// They start at the address kSEND_CMDS_END defined ^^ above as 004
messengerCallbackFunction messengerCallbacks[] = 
{
  bens_msg,            // 004 in this example
  jerrys_base64_data,  // 005
  NULL
};

// ------------------ C A L L B A C K  M E T H O D S -------------------------

void bens_msg()
{
  // Message data is any ASCII bytes (0-255 value). But can't contain the field
  // separator, command separator chars you decide (eg ',' and ';')
  cmdMessenger.sendCmd(kACK,"bens cmd recieved");
  while ( cmdMessenger.available() )
  {
    char buf[350] = { '\0' };
    cmdMessenger.copyString(buf, 350);
    if(buf[0])
      cmdMessenger.sendCmd(kACK, buf);
  }
}

void jerrys_base64_data()
{
  // To avoid conflicts with the control characters and any newline characters
  // Message length increases about 30%-40%

  // Afer base64_decode(), we just parse the buffer and unpack it into your
  // target / desination data type eg bitmask, float, double, whatever.
  char buf[350] = { '\0' };
  boolean data_msg_printed = false;
  cmdMessenger.sendCmd(kACK,"jerrys cmd recieved");

  // base64 decode
  while ( cmdMessenger.available() )
  {
    if(!data_msg_printed)
    {
      cmdMessenger.sendCmd(kACK, "what you send me, decoded base64...");
      data_msg_printed = true;
    }
    char buf[350] = { '\0' };
    cmdMessenger.copyString(buf, 350);
    if(buf[0])
    {
      char decode_buf[350] = { '\0' };
      base64_decode(decode_buf, buf, 350);
      cmdMessenger.sendCmd(kACK, decode_buf);
    }
  }

  // base64 encode
  if(!data_msg_printed)
  {
    cmdMessenger.sendCmd(kACK, "\"the bears are allright\" encoded in base64...");
    char base64_msg[350] = { '\0' };
    base64_encode(base64_msg, "the bears are allright", 22);
    cmdMessenger.sendCmd(kACK, base64_msg);
  }
}

// ------------------ D E F A U L T  C A L L B A C K S -----------------------

void arduino_ready()
{
  // In response to ping. We just send a throw-away Acknowledgement to say "im alive"
  cmdMessenger.sendCmd(kACK,"Arduino ready");
}

void unknownCmd()
{
  // Default response for unknown commands and corrupt messages
  cmdMessenger.sendCmd(kERR,"Unknown command");
}

// ------------------ E N D  C A L L B A C K  M E T H O D S ------------------



// ------------------ S E T U P ----------------------------------------------

void attach_callbacks(messengerCallbackFunction* callbacks)
{
  int i = 0;
  int offset = kSEND_CMDS_END;
  while(callbacks[i])
  {
    cmdMessenger.attach(offset+i, callbacks[i]);
    i++;
  }
}

void setup() 
{
  // Listen on serial connection for messages from the pc
  // Serial.begin(57600);  // Arduino Duemilanove
  Serial.begin(115200); // Arduino Uno

  // cmdMessenger.discard_LF_CR(); // If your PC tty sends newlines
  cmdMessenger.print_LF_CR();   // Use when debugging
  
  // Attach default / generic callback methods
  cmdMessenger.attach(kARDUINO_READY, arduino_ready);
  cmdMessenger.attach(unknownCmd);

  // Attach my application's user-defined callback methods
  attach_callbacks(messengerCallbacks);

  arduino_ready();
}


// ------------------ M A I N ( ) --------------------------------------------

// Timeout handling
long timeoutInterval = 2000; // 2 seconds
long previousMillis = 0;
int counter = 0;

void timeout()
{
  if (/* condition */ true)
    ; // Handle event
  counter ++;
}  

void loop() 
{
  // Process incoming serial data, if any
  cmdMessenger.feedinSerialData();

  // handle timeout function, if any
  if (  millis() - previousMillis > timeoutInterval )
  {
    timeout();
    previousMillis = millis();
  }

  // Loop.
}

// ------------------ S E R I A L  M O N I T O R -----------------------------
// 
// Try typing these command messages in the serial monitor!
// 
// 4,hi,heh,ho!;
// 5;
// 5,dGhlIGJlYXJzIGFyZSBhbGxyaWdodA==;
// 5,dGhvc2UgbmFzdHkgY29udHJvbCA7OyBjaGFyYWN0ZXJzICws==;
// 2;
// 6;
// 
// 
// Expected output:
// 
// 1,Arduino ready
// 1,bens cmd recieved
// 1,hi
// 1,heh
// 1,ho!
// 1,jerrys cmd recieved
// 1,"the bears are allright" encoded in base64...
// 1,dGhlIGJlYXJzIGFyZSBhbGxyaWdodA==
// 1,jerrys cmd recieved
// 1,what you send me, decoded base64...
// 1,the bears are allright
// 1,jerrys cmd recieved
// 1,what you send me, decoded base64...
// 1,those nasty control ;; characters ,,
// 1,Arduino ready
// 3,Unknown command
// 


