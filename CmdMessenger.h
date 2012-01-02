#ifndef CmdMessenger_h
#define CmdMessenger_h

#include <inttypes.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include "Stream.h"

extern "C" {
  // Our callbacks are always method signature: void cmd(void);
  typedef void (*messengerCallbackFunction)(void);
}

#define MAXCALLBACKS 50        // The maximum number of unique commands
#define MESSENGERBUFFERSIZE 64 // The maximum length of the buffer (defaults to 64)
#define DEFAULT_TIMEOUT 5000   // Abandon incomplete messages if nothing heard after 5 seconds

class CmdMessenger
{  

protected:
  uint8_t bufferIndex;     // Index where to write the data
  uint8_t bufferLength;    // Is set to MESSENGERBUFFERSIZE
  uint8_t bufferLastIndex; // The last index of the buffer

  messengerCallbackFunction default_callback;
  messengerCallbackFunction callbackList[MAXCALLBACKS];

  // (not implemented, generally not needed)
  // when we are sending a message and requre answer or acknowledgement
  // suspend any processing (process()) when serial intterupt is recieved
  // Even though we usually only have single processing thread we still need
  // this i think because Serial interrupts.
  // Could also be usefull when we want data larger than MESSENGERBUFFERSIZE
  // we could send a startCmd, which could pauseProcessing and read directly
  // from serial all the data, send acknowledge etc and then resume processing  
  boolean pauseProcessing;
    
  void handleMessage(); 
  void init(Stream &comms, char field_separator, char command_separator);
  uint8_t process(int serialByte);
  void reset();

  char buffer[MESSENGERBUFFERSIZE]; // Buffer that holds the data
  uint8_t messageState;
  uint8_t dumped;
  char* current; // Pointer to current data
  char* last;

public:
  CmdMessenger(Stream &comms);
  CmdMessenger(Stream &comms, char fld_separator);
  CmdMessenger(Stream &comms, char fld_separator, char cmd_separator);

  void attach(messengerCallbackFunction newFunction);
  void discard_LF_CR();
  void print_LF_CR();

  uint8_t next();
  uint8_t available();

  int readInt();
  char readChar();
  void copyString(char *string, uint8_t size);
  uint8_t checkString(char *string);

  // Polymorphism used to interact with serial class
  // Stream is an abstract base class which defines a base set
  // of functionality used by the serial classes.
  Stream *comms;
  
  void attach(byte msgId, messengerCallbackFunction newFunction);
  
  char* sendCmd(int cmdId, char *msg, boolean reqAc = false, 
		    char *replyBuff = NULL, int butSize = 0, int timeout = DEFAULT_TIMEOUT, 
		    int retryCount = 10);

  void feedinSerialData();
  
  char command_separator;
  char field_separator;

  boolean discard_newlines;
  boolean print_newlines;

  boolean blockedTillReply(int timeout = DEFAULT_TIMEOUT);
};
#endif