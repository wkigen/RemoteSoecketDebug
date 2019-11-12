#include "Debugger.h"
#include "system_utils.h"
#include "fake_dlfcn.h"
#include <string>
#include <dlfcn.h>

namespace JDWP {
    enum JdwpTransportType {
        kJdwpTransportNone = 0,
        kJdwpTransportUnknown,      // Unknown tranpsort
        kJdwpTransportSocket,       // transport=dt_socket
        kJdwpTransportAndroidAdb,   // transport=dt_android_adb
    };
    struct JdwpOptions {
        JdwpTransportType transport = kJdwpTransportNone;
        bool server = false;
        bool suspend = false;
        std::string host = "";
        uint16_t port = static_cast<uint16_t>(-1);
    };
}

namespace SocketDebug{

    Debugger::Debugger(){

    }

    Debugger::~Debugger(){

    }

    bool Debugger::openJDWP(){

        void *handler = fake_dlopen("/system/lib/libart.so", RTLD_NOW);

        if(handler == NULL){
            return false;
        }

        //对于debuggable false的配置，重新设置为可调试
        void (*allowJdwp)(bool);
        allowJdwp = (void (*)(bool)) fake_dlsym(handler, "_ZN3art3Dbg14SetJdwpAllowedEb");
        allowJdwp(true);

        void (*pfun)();
        //关闭之前启动的jdwp-thread
        pfun = (void (*)()) fake_dlsym(handler, "_ZN3art3Dbg8StopJdwpEv");
        pfun();

        if(getAndroidSdk() < ANDROID_O){
            //重新配置gJdwpOptions
            bool (*parseJdwpOptions)(const std::string&);
            parseJdwpOptions = (bool (*)(const std::string&)) fake_dlsym(handler,
                                                                         "_ZN3art3Dbg16ParseJdwpOptionsERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE");
            std::string options = "transport=dt_socket,address=8000,server=y,suspend=n";
            parseJdwpOptions(options);
        } else{
            //重新配置gJdwpOptions
            JDWP::JdwpOptions jdwp_options;
            bool (*parseJdwpOptions)(const std::string&, JDWP::JdwpOptions* jdwp_options);
            parseJdwpOptions = (bool (*)(const std::string&, JDWP::JdwpOptions* jdwp_options)) fake_dlsym(handler,
                                                                                                          "_ZN3art4JDWP16ParseJdwpOptionsERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEPNS0_11JdwpOptionsE");
            std::string options = "transport=dt_socket,address=8000,server=y,suspend=n";
            parseJdwpOptions(options,&jdwp_options);

            //设置重新配置gJdwpOptions
            void (*ConfigureJdwp)(const JDWP::JdwpOptions& jdwp_options);
            ConfigureJdwp = (void (*)(const JDWP::JdwpOptions& jdwp_options)) fake_dlsym(handler, "_ZN3art3Dbg13ConfigureJdwpERKNS_4JDWP11JdwpOptionsE");
            ConfigureJdwp(jdwp_options);
        }

        //重新startJdwp
        pfun = (void (*)()) fake_dlsym(handler, "_ZN3art3Dbg9StartJdwpEv");
        pfun();


        jdwpProtocol.attachProcess();

        return true;

    }
}
