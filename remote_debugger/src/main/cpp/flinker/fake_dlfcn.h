#ifndef REMOTESOECKETDEBUG_FAKE_DLFUN_H
#define REMOTESOECKETDEBUG_FAKE_DLFUN_H

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <elf.h>
#include <android/log.h>
#include <malloc.h>

#define TAG_NAME	"fake_dlfun"

#define log_info(fmt,args...) __android_log_print(ANDROID_LOG_INFO, TAG_NAME, (const char *) fmt, ##args)
#define log_err(fmt,args...) __android_log_print(ANDROID_LOG_ERROR, TAG_NAME, (const char *) fmt, ##args)

#ifdef __cplusplus
extern "C" {
#endif

struct ctx {
    void *load_addr;
    void *dynstr;
    void *dynsym;
    int nsyms;
    off_t bias;
};

void *fake_dlopen(const char *libpath, int flags);

int fake_dlclose(void *handle);

void *fake_dlsym(void *handle, const char *name);



#ifdef __cplusplus
} // extern "C"
#endif


#endif //REMOTESOECKETDEBUG_FAKE_DLFUN_H
