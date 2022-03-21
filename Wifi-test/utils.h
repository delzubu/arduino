#ifndef DELZUBU_UTILS_H_
#define DELZUBU_UTILS_H_ 

#include <WString.h>

typedef void (*cmdCallback)(String);
typedef void (*commandHandler)(String, String);

void spinner(); 
void consoleHandler();
void consoleHandler(commandHandler);
void registerCommandCallback(String, cmdCallback);

#endif /* DELZUBU_UTILS_H_ */ 