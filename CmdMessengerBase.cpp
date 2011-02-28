

//ADDED FOR COMPATIBILITY WITH WIRING
extern "C" {
  #include <stdlib.h>
}
#include "Messenger.h"

void Messenger::attach(messengerCallbackFunction newFunction) {
	callback = newFunction;
}

void Messenger::reset() {
        bufferIndex = 0;
        current = NULL;
        last = NULL;
        dumped = 1;
}

int Messenger::readInt() {

    if (next()) {
    	dumped = 1;
    	return atoi(current);
     }
  return 0;

}

char Messenger::readChar() {

  if (next()) {
  	 dumped = 1;
  	 return current[0];
 }
  return 0;

}

void Messenger::copyString(char *string, uint8_t size) {
	
	if (next()) {
		dumped = 1;
		strlcpy(string,current,size);
	} else {
		if ( size ) string[0] = '\0';
	}
}

uint8_t Messenger::checkString(char *string) {
	
	if (next()) {
		if ( strcmp(string,current) == 0 ) {
			dumped = 1;
			return 1;
		} else {
			return 0;
		}
	} 
}



uint8_t Messenger::next() {

  char * temppointer= NULL;
  switch (messageState) {
  case 0:
    return 0;
  case 1:
    temppointer = buffer;
    messageState = 2;
  default:
    if (dumped) current = strtok_r(temppointer,token,&last);
    if (current != NULL) {
    	dumped = 0;
    	return 1; 
	}
  }
  
  return 0;
}

uint8_t Messenger::available() {
	
  return next();
  
}

