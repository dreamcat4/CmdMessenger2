

//ADDED FOR COMPATIBILITY WITH WIRING
extern "C"
{
  #include <stdlib.h>
}
#include "CmdMessengerBase.h"

// Not sure if it will work for signed.. check it out
/*unsigned char *CmdMessengerBase::writeRealInt(int val, unsigned char buff[2])
{
  buff[1] = (unsigned char)val;
  buff[0] = (unsigned char)(val >> 8);  
  buff[2] = 0;
  return buff;
}

char* CmdMessengerBase::writeRealLong(long val, char buff[4])
{
  //buff[1] = (unsigned char)val;
  //buff[0] = (unsigned char)(val >> 8);  
  return buff;
}

char* CmdMessengerBase::writeRealFloat(float val, char buff[4])
{
  //buff[1] = (unsigned char)val;
  //buff[0] = (unsigned char)(val >> 8);  
  return buff;
}
*/

int CmdMessengerBase::readInt()
{
  if (next())
  {
    dumped = 1;
    return atoi(current);
  }
  return 0;
}

char CmdMessengerBase::readChar()
{
  if (next())
  {
    dumped = 1;
    return current[0];
  }
  return 0;
}

void CmdMessengerBase::copyString(char *string, uint8_t size)
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

uint8_t CmdMessengerBase::checkString(char *string)
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

uint8_t CmdMessengerBase::next()
{
  char * temppointer= NULL;
  switch (messageState)
  {
    case 0:
    return 0;
    case 1:
    temppointer = buffer;
    messageState = 2;
    default:
    if (dumped)
      current = strtok_r(temppointer,token,&last);
    if (current != NULL)
    {
      dumped = 0;
      return 1; 
    }
  }
  return 0;
}

uint8_t CmdMessengerBase::available()
{
  return next();
}

