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
#ifndef _TICONV_H_
#define _TICONV_H_

#include <string>

namespace mpl
{
    std::string toUTF8(const std::string &str);
    std::string toGBK(const std::string &str);
    // Get encoding from `iconv --list` 
    std::string iconvert(const std::string &fromCode, const std::string &toCode, const std::string &str);
} // namespace mpl


#endif /* _TICONV_H_ */
