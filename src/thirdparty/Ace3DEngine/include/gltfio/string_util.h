//
// Created by paynepan on 2020/8/12.
//

#ifndef TNT_STRING_UTIL_H
#define TNT_STRING_UTIL_H

#include <string>

LIGHT3D_NAMESPACE_BEGIN
namespace string_util {

static bool hasEnding(std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

static bool startWith(const std::string &fullString, const std::string &ending) {
    return fullString.find(ending) == 0;
}

}
LIGHT3D_NAMESPACE_END

#endif //TNT_STRING_UTIL_H
