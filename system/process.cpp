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
#include "process.h"

namespace mpl
{
std::string process(const std::string &command) {
    char output[1024 * 64] = {0};
    FILE *pipofile = popen(command.c_str(), "r");
    fread(output, 1, sizeof(output) - 1, pipofile);
    pclose(pipofile);

    return std::string(output);
}

//更安全调用system
typedef void (*sighandler_t)(int);
int pox_system(const std::string &cmd) {
    int ret = 0;
    sighandler_t old_handler;

    old_handler = signal(SIGCHLD, SIG_DFL);
    ret = system(cmd.c_str());
    signal(SIGCHLD, old_handler);

    return ret;
}
} // namespace mpl
