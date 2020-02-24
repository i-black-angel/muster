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
#include "muuid.h"
#include "muuid_p.h"

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

namespace mpl {

std::string uuidgen()
{
    char buf[64] = {0};
#ifdef M_OS_WIN
    /**
     * should be included <objbase.h>
     */
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid)) {
		_snprintf_s(buf, sizeof(buf) -1,
					"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
					guid.Data1,
					guid.Data2,
					guid.Data3,
					guid.Data4[0], guid.Data4[1],
					guid.Data4[2], guid.Data4[3],
					guid.Data4[4], guid.Data4[5],
					guid.Data4[6], guid.Data4[7]);
	}
#else
    /**
     * should be included <uuid/uuid.h> and link with -luuid,
     * BUT HEY, i load libuuid on this library, :D
     */
	uuid_t guid;
	uuid_generate(guid);
	uuid_unparse(guid, buf);
#endif
	return std::string(buf);
}

} // namespace reco

#ifdef _MSC_VER
# pragma warning (pop)
#endif
