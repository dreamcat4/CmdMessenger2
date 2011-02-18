
//ADDED FOR COMPATIBILITY WITH WIRING
extern "C" {
  #include <stdlib.h>
}
#include "CmdMessenger.h"
//#include "Streaming.h"

//////////////////// Cmd Messenger imp ////////////////
CmdMessenger::CmdMessenger(SerialBase &ccomms) : Messenger()
{
  comms = &ccomms;
  init();
}

CmdMessenger::CmdMessenger(SerialBase &ccomms, char separator) : Messenger(separator) 
{
  comms = &ccomms;
  init();
}

void CmdMessenger::attach(byte msgId, messengerCallbackFunction newFunction) {
    if (msgId > 0 && msgId <= MAXCALLBACKS) // <= ? I think its ok ?
	callbackList[msgId-1] = newFunction;
}

void CmdMessenger::init()
{	
  for (int i = 0; i < MAXCALLBACKS; i++)
    callbackList[i] = NULL;
  pauseProcessing = false;
}

void CmdMessenger::handleMessage()
{
	int id = readInt();
	//Serial << "ID+" << id << endl;
	// Because readInt() can fail and return a 0 we can't
	// start our array index at that number
	if (id > 0 && id < MAXCALLBACKS && callbackList[id-1] != NULL) 
	  (*callbackList[id-1])();
	else // Cmd not registered default callback
	  (*callback)();
}

void CmdMessenger::feedinSerialData()
{
  while ( !pauseProcessing && comms->available( ) ) 
    process(comms->read( ) );
}

// Taken from RougueMP3 library
int8_t CmdMessenger::_read_blocked(void)
{
  // int8_t r;
  
  while(!comms->available());
  // while((r = this->_readf()) < 0);   // this would be faster if we could guarantee that the _readf() function
                                        // would return -1 if there was no byte read
  return comms->read();
}

boolean CmdMessenger::blockedTillReply(int timeout)
{
  unsigned long start = millis();
  unsigned long time = start;
  while(!comms->available() || (start - time) > timeout )
    time = millis();
}

// Not sure if it will work for signed.. check it out
/*unsigned char *CmdMessenger::writeRealInt(int val, unsigned char buff[2])
{
  buff[1] = (unsigned char)val;
  buff[0] = (unsigned char)(val >> 8);  
  buff[2] = 0;
  return buff;
}

char* CmdMessenger::writeRealLong(long val, char buff[4])
{
  //buff[1] = (unsigned char)val;
  //buff[0] = (unsigned char)(val >> 8);  
  return buff;
}

char* CmdMessenger::writeRealFloat(float val, char buff[4])
{
  //buff[1] = (unsigned char)val;
  //buff[0] = (unsigned char)(val >> 8);  
  return buff;
}
*/

// if the arguments in the future could be passed in as int/long/float etc
// then it might make sense to use the above writeReal????() methods
// I've removed them for now.
char* CmdMessenger::sendCmd(int cmdId, char *msg, boolean reqAc, 
			       char *replyBuff, int butSize, int timeout, 
			       int retryCount)
{
  int tryCount = 0;  
  pauseProcessing = true;
  //*comms << cmdId << token[0] << msg << endl;
  comms->print(cmdId); 
  comms->print(token[0]); 
  comms->print(msg); 
  comms->println();
  if (reqAc) {    
    do {
      blockedTillReply(timeout);
      //strcpy(replyBuff, buf;
    } while( tryCount < retryCount);
  }
  
  pauseProcessing = false;
  return NULL;
}