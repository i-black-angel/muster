/**
 * Copyright 2020 Shusheng Shao <iblackangel@163.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _SYSTEM_TEMPL_H_
#define _SYSTEM_TEMPL_H_

#include <signal.h>
#include <stdio.h>
#include <string>

namespace mpl {
template <typename... Args>
std::string xsprintf(const char *fmt, Args... args) {
    std::string buffer;
    size_t len = snprintf(NULL, 0, fmt, args...);
    buffer.resize(len);
    snprintf(&buffer[0], len + 1, fmt, args...);
    return buffer;
}

std::string process(const std::string &cmd);
int pox_system(const std::string &cmd);
}  // namespace mpl

#endif /* _SYSTEM_TEMPL_H_ */
