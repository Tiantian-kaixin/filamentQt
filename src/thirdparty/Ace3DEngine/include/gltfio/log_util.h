//
// Created by kevinxing on 2020-03-04.
//

#ifndef TNT_LOG_UTIL_H
#define TNT_LOG_UTIL_H

#if (FILAMENT_ENV_IOS || (IOS))
    #include <iostream>
    #define LOGD std::cout
    #define LOGE std::cerr
    #define LOGEND std::endl
#else
    #include <utils/Log.h>
    #define LOGD utils::slog.d
    #define LOGE utils::slog.e
    #define LOGEND utils::io::endl
#endif

#endif //TNT_LOG_UTIL_H
