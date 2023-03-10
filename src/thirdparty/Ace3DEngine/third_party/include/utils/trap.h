/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UTILS_TRAP_H
#define UTILS_TRAP_H

#include <csignal>

#if defined(WIN32)
#include <Windows.h>
#include <utils/unwindows.h>
#endif

namespace utils {

// This can be used as a programmatic breakpoint.
inline void debug_trap() noexcept {
#if defined(WIN32)
    DebugBreak();
#else
    //std::raise(SIGINT);
#endif
}

} // namespace utils

#endif // UTILS_TRAP_H
