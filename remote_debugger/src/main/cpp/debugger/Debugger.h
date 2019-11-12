#ifndef REMOTESOECKETDEBUG_DEBUGGER_H
#define REMOTESOECKETDEBUG_DEBUGGER_H

#include "JdwpProtocol.h"

namespace SocketDebug{

     class Debugger {
     public:

        Debugger();

        ~Debugger();

        bool openJDWP();

     private:

         JdwpProtocol  jdwpProtocol;
    };

}

#endif //REMOTESOECKETDEBUG_DEBUGGER_H
