
// ADDED FOR COMPATIBILITY WITH WIRING ??
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

CmdMessenger::CmdMessenger(Stream &ccomms, char fld_separator)
{
  init(ccomms,fld_separator,';');
}

CmdMessenger::CmdMessenger(Stream &ccomms, char fld_separator, char cmd_separator)
{
  init(ccomms,fld_separator,cmd_separator);
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

void CmdMessenger::init(Stream &ccomms, char fld_separator, char cmd_separator)
{
  comms = &ccomms;
  
  discard_newlines = false;
  print_newlines   = false;

  field_separator   = fld_separator;
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

uint8_t CmdMessenger::next()
{
  char * temppointer= NULL;
  // Currently, cmd messenger only supports 1 char for the field seperator
  const char seperator_tokens[] = { field_separator,'\0' };
  switch (messageState)
  {
    case 0:
    return 0;
    case 1:
    temppointer = buffer;
    messageState = 2;
    default:
    if (dumped)
      current = strtok_r(temppointer,seperator_tokens,&last);
    if (current != NULL)
    {
      dumped = 0;
      return 1; 
    }
  }
  return 0;
}

uint8_t CmdMessenger::available()
{
  return next();
}

uint8_t CmdMessenger::process(int serialByte) {
    messageState = 0;
    char serialChar = (char)serialByte;

    if (serialByte > 0) {

      // Currently, cmd messenger only supports 1 char for the command seperator
      if(serialChar == command_separator)
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

        if(discard_newlines && (serialChar != field_separator))
          if((serialChar == '\n') || (serialChar == '\r'))
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
  //*comms << cmdId << field_separator << msg << endl;
  comms->print(cmdId);
  comms->print(field_separator);
  comms->print(msg);
  comms->print(command_separator);
  if(print_newlines)
    comms->println(); // should append BOTH \r\n
  if (reqAc) {    
    do {
      blockedTillReply(timeout);
      //strcpy(replyBuff, buf;
    } while( tryCount < retryCount);
  }
  
  pauseProcessing = false;
  return NULL;
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

int CmdMessenger::readInt()
{
  if (next())
  {
    dumped = 1;
    return atoi(current);
  }
  return 0;
}

char CmdMessenger::readChar()
{
  if (next())
  {
    dumped = 1;
    return current[0];
  }
  return 0;
}

void CmdMessenger::copyString(char *string, uint8_t size)
{
  if (next())
  {
    dumped = 1;
    strlcpy(string,current,size);
  }
  else
  {
    if ( size ) string[0] = '\0';
  }
}

uint8_t CmdMessenger::checkString(char *string)
{
  if (next())
  {
    if ( strcmp(string,current) == 0 )
    {
      dumped = 1;
      return 1;
    }
    else
    {
      return 0;
    }
  } 
}
