// See 1..MAXCALLBACKS defined in CmdMessenger.h for available
// id's for commands NB Id's of 0 can't be used and will crash things

// Put the cmds which are handled by arduino here
// They will be called from TouchShield
#define BUTPRESS_ARDUINO_CMD 1
#define MESSAGE_ARDUINO_CMD 2


// Put the cmds which are handled by TouchShield here
// They will be called from Arduino
#define DRAWELLIPSE_TOUCH_CMD 1
#define MESSAGE_TOUCH_CMD 2