

//ADDED FOR COMPATIBILITY WITH WIRING
extern "C" {
  #include <stdlib.h>
}
#include "CmdMessengerBase.h"

void CmdMessengerBase::reset() {
        bufferIndex = 0;
        current = NULL;
        last = NULL;
        dumped = 1;
}

int CmdMessengerBase::readInt() {

    if (next()) {
    	dumped = 1;
    	return atoi(current);
     }
  return 0;

}

char CmdMessengerBase::readChar() {

  if (next()) {
  	 dumped = 1;
  	 return current[0];
 }
  return 0;

}

void CmdMessengerBase::copyString(char *string, uint8_t size) {
	
	if (next()) {
		dumped = 1;
		strlcpy(string,current,size);
	} else {
		if ( size ) string[0] = '\0';
	}
}

uint8_t CmdMessengerBase::checkString(char *string) {
	
	if (next()) {
		if ( strcmp(string,current) == 0 ) {
			dumped = 1;
			return 1;
		} else {
			return 0;
		}
	} 
}



uint8_t CmdMessengerBase::next() {

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

uint8_t CmdMessengerBase::available() {
	
  return next();
  
}

