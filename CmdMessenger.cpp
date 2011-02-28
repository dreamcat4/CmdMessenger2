
//ADDED FOR COMPATIBILITY WITH WIRING
extern "C" {
  #include <stdlib.h>
}
#include "CmdMessenger.h"
#include <Streaming.h>

//////////////////// Cmd Messenger imp ////////////////
CmdMessenger::CmdMessenger(Stream &ccomms)
{
  init(ccomms,' ',';');
}

CmdMessenger::CmdMessenger(Stream &ccomms, char field_separator)
{
  init(ccomms,field_separator,';');
}

CmdMessenger::CmdMessenger(Stream &ccomms, char field_separator, char cmd_separator)
{
  init(ccomms,field_separator,cmd_separator);
}

void CmdMessenger::attach(messengerCallbackFunction newFunction) {
	default_callback = newFunction;
}

void CmdMessenger::attach(byte msgId, messengerCallbackFunction newFunction) {
    if (msgId > 0 && msgId <= MAXCALLBACKS) // <= ? I think its ok ?
	callbackList[msgId-1] = newFunction;
}

void CmdMessenger::discard_LF_CR()
{
  discard_newlines = true;
}

void CmdMessenger::print_LF_CR()
{
  print_newlines   = true;
}

void CmdMessenger::init(Stream &ccomms, char field_separator, char cmd_separator)
{
  comms = &ccomms;
  
  discard_newlines = false;
  print_newlines   = false;

  token[0] = field_separator;
  token[1] = '\0';
  command_separator = cmd_separator;

  bufferLength = MESSENGERBUFFERSIZE;
  bufferLastIndex = MESSENGERBUFFERSIZE -1;
  reset();

  default_callback = NULL;
  for (int i = 0; i < MAXCALLBACKS; i++)
    callbackList[i] = NULL;

  pauseProcessing = false;
}

void CmdMessenger::reset() {
        bufferIndex = 0;
        current = NULL;
        last = NULL;
        dumped = 1;
}

uint8_t CmdMessenger::process(int serialByte) {
    messageState = 0;
    if (serialByte > 0) {

      if((char)serialByte == command_separator)
      {
        buffer[bufferIndex]=0;
        if(bufferIndex > 0)
        {
          messageState = 1;
          current = buffer;
        }
        reset();
      }
      else
      {
        buffer[bufferIndex]=serialByte;
        bufferIndex++;
        if (bufferIndex >= bufferLastIndex) reset();

        if(discard_newlines)
          if(((char)serialByte == '\n') || ((char)serialByte == '\r'))
            reset();
      }
    }

    if ( messageState == 1 ) {
    	handleMessage();
    }
    return messageState;
}

void CmdMessenger::handleMessage()
{
    // If we didnt want to use ASCII integer...
    // we would change the line below vv
	int id = readInt();

	//Serial << "ID+" << id << endl;
	// Because readInt() can fail and return a 0 we can't
	// start our array index at that number
	if (id > 0 && id <= MAXCALLBACKS && callbackList[id-1] != NULL)
	  (*callbackList[id-1])();
	else // Cmd not registered default callback
	  (*default_callback)();
}

void CmdMessenger::feedinSerialData()
{
  while ( !pauseProcessing && comms->available( ) ) 
    process(comms->read( ) );
}

boolean CmdMessenger::blockedTillReply(int timeout)
{
  unsigned long start = millis();
  unsigned long time = start;
  while(!comms->available() || (start - time) > timeout )
    time = millis();
}

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
  if(print_newlines)
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