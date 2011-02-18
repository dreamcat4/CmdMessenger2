#ifndef CmdMessenger_h
#define CmdMessenger_h

#include <inttypes.h>
#include "WProgram.h"

#include "Messenger.h"
#include "SerialBase.h"

#define MAXCALLBACKS 50 
#define DEFAULT_TIMEOUT 5000

class CmdMessenger : public Messenger
{  
  messengerCallbackFunction callbackList[MAXCALLBACKS];
 // messengerReplyCallbackFunction replyCallback; 
 
  // when we are sending a message and requre answer or acknowledgement
  // suspend any processing (process()) when serial intterupt is recieved
  // Even though we usually only have single processing thread we still need
  // this i think because Serial interrupts.
  // Could also be usefull when we want data larger than MESSENGERBUFFERSIZE
  // we could send a startCmd, which could pauseProcessing and read directly
  // from serial all the data, send acknowledge etc and then resume processing  
  boolean pauseProcessing;
    
  void handleMessage(); 
  void init();  
    
public:
  CmdMessenger(SerialBase &comms);
  CmdMessenger(SerialBase &comms, char separator);    
  
  // Polymorphism used to interact with serial class
  // SerialBase is an abstract base class which defines a base set
  // of functionality for serial classes.
  SerialBase *comms;
  
  void attach(byte msgId, messengerCallbackFunction newFunction);
  
  char* sendCmd(int cmdId, char *msg, boolean reqAc = false, 
		    char *replyBuff = NULL, int butSize = 0, int timeout = DEFAULT_TIMEOUT, 
		    int retryCount = 10);
  // To be put in main loop instead of the 
  //while ( Serial.available( ) ) cmdMessage.process(Serial.read( ) );
  // Although you can do iether, this just makes sure that the SerialBase
  // passed in through constructor is the same as used in main loop
  void feedinSerialData();
  
  int8_t _read_blocked(void);
  boolean blockedTillReply(int timeout = DEFAULT_TIMEOUT);
};
#endif