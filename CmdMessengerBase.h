
#ifndef Messenger_h
#define Messenger_h
#define MESSENGERBUFFERSIZE 64

#include <inttypes.h>
#include "WProgram.h"

extern "C" {
// callback function
    typedef void (*messengerCallbackFunction)(void);
}

class CmdMessengerBase
{
protected:

  char buffer[MESSENGERBUFFERSIZE]; // Buffer that holds the data

  uint8_t messageState;
  
  char* current; // Pointer to current data
  char* last;
  
  char token[2];
  uint8_t dumped;
  

public:
  uint8_t next();

  int readInt();
  char readChar();
  void copyString(char *string, uint8_t size);
  uint8_t checkString(char *string);
  
};


#endif

