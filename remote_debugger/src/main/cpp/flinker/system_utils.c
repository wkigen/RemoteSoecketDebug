#include "system_utils.h"
#include <stdlib.h>
#include <sys/system_properties.h>


int getAndroidSdk() {
   char sdk_version_str[PROP_VALUE_MAX];
   __system_property_get("ro.build.version.sdk", sdk_version_str);
   return atoi(sdk_version_str);
}

