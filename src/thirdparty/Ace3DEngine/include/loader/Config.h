//
//  Config.h
//  brick
//
//  Created by louisluo on 13/02/2017.
//  Copyright © 2017 louisluo. All rights reserved.
//

#ifndef Config_h
#define Config_h

#include <string.h>

#ifdef __APPLE__
#include <TargetConditionals.h>
#ifdef TARGET_OS_IPHONE
#define BK_PLATFORM_IOS
#endif

#if defined(QQ_RDM_VERSION) || defined(QQ_GRAY_VERSION) || defined(QQ_APPSTORE_VERSION)
#undef BK_DEV_ENVIROMENT
#else
#define BK_DEV_ENVIROMENT
#endif
#endif


#ifdef __ANDROID__
#define BK_PLATFORM_IOS
//为了走tlvreader中GetValueArray里ios的逻辑改的，如果以后需要区分，则对应那段逻辑要加入
//同样，加入那段逻辑的话，析构函数的逻辑也要对应加入
#endif

#if defined(_WIN32) || defined(_WIN64)
#define BK_PLATFORM_QT
#endif

#ifdef Q_OS_MACOS
#define BK_PLATFORM_QT
#endif

#if defined(__ANDROID__) || defined(__APPLE__) || defined(BK_PLATFORM_QT)
#define BK_INLINE_SHADER
#endif//


#define JSC_SCRIPT 1

#define BK_USE_REALLOC 1

//#define BK_DEBUG 1     //打开后，js语法错误时会abort

#define BK_ENABLE_MQQ 1

#define BK_BATCH_RENDER_ENABLE 1 // 批提交渲染

#define BK_ENABLE_RENDER_TREE_DETECT 1 // 启用渲染树检测

//#define BK_ENABLE_MTU_RENDER    1 // 多重纹理渲染

#define BK_ENABLE_CLIP_SCREEN   1 // 屏幕裁剪检测

#ifdef __APPLE__
    #ifdef DEBUG
        #define BK_CHIPMUNK_DEBUG 1 //打开后会显示包围盒。
    #endif
#endif

//#define BK_ENABLE_DIAGNOSTICS 1

//#define BK_ENABLE_QAV 1 //音视频

//#define BK_ENABLE_MONITOR_LOG 1 // 打开监控日志


#if defined(__APPLE__) && defined(TARGET_IPHONE_SIMULATOR)
//    #define USE_SKIA
#endif


#ifdef __ANDROID__
	#define USE_SKIA
#endif

#define BK_ENABLE_WEBGL

// 用于打印 os 或者 .a 的版本号，在脚本编译 so 或者 .a 的时候，会自动获取 git 的版本替换进来
#define SO_VERSION "1.000"

#endif /* Config_h */



