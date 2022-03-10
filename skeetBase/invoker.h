//
//  invoker.h
//  Skeet
//
//  Created by Journey Curtis on 3/9/22.
//

#ifndef invoker_h
#define invoker_h

#include "command.h"

/***********************************************
 * SKEET :: INVOKER
 * Invoker keeps a pointer to a command.
 * Dynamically set the pointer using setCommand
 * and execute the command using executeCommand.
 **********************************************/
class Invoker {
private:
   AbstractCommand * command;
   
public:
   void setCommand(AbstractCommand * command) { this->command = command; }
   void executeCommand() { command->execute(); }
};

#endif /* invoker_h */
