#include "library.h"

namespace Lib
{

    int ExportFunc()
    {
        __android_log_write(ANDROID_LOG_INFO, "CPP", "ExportFunc called.");
        return 69420;
    }

}