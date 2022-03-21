#include "utils.h" 
#include <HardwareSerial.h>
#include <map>                 // Part of C+

static char const spin_chars[] = "/-\\|";
static int spin_pos = 0;

void cmd_help(String);

std::map<String, cmdCallback> command_callback_map = { {"help", &cmd_help }};

void spinner() {
    spin_pos = (spin_pos+1) % sizeof(spin_chars);
    Serial.print(spin_chars[spin_pos]);
    // Serial.print('\b');
}

void defaultCommandHandler(String, String);

void consoleHandler() {
  consoleHandler(&defaultCommandHandler);
}

void consoleHandler(commandHandler cmdHandler) {
  if (Serial.available() > 0) {
    // read the incoming string:
    String incomingString = Serial.readString();
    incomingString.trim();

    String cmdStr;
    String argStr;
    int i = incomingString.indexOf(" ");

    if(i == -1) {
      cmdStr = incomingString;
      argStr = "";
    } else {
      cmdStr = incomingString.substring(0, i);
      argStr = incomingString.substring(i+1, incomingString.length());
    }
    if(command_callback_map[cmdStr]) {
      command_callback_map[cmdStr](argStr);
    } else if(cmdHandler) {
      cmdHandler(cmdStr, argStr);
    }
  }
}

void registerCommandCallback(String key, cmdCallback callback) {
  command_callback_map[key] = callback;
}

void defaultCommandHandler(String a, String b) {

}

void cmd_help(String args) {
  Serial.println("Available commands:");
  for (auto &item : command_callback_map) {
        Serial.println(item.first);
  }
}