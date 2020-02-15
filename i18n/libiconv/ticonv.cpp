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
#include <string.h>
#include <string>
#include <iconv.h>
#include <fcntl.h>
#include <unistd.h>
#include "ticonv.h"

namespace mpl {

std::string toUTF8(const std::string &str) {
    return iconvert("GB18030", "UTF-8", str);
}

std::string toGBK(const std::string &str) {
    return iconvert("UTF-8", "GB18030", str);
}

// Get encoding from `iconv --list`
std::string iconvert(const std::string &fromCode, const std::string &toCode,
                     const std::string &str) {
    char *inbuf = new char[str.size() + 1];
    size_t inbytes = str.size();
    strcpy(inbuf, str.c_str());

    char *outbuf = new char[str.size() * 2 + 1];  // twice of input stream
    size_t outbytes = str.size() * 2;
	memset(outbuf, 0x00, outbytes + 1); // init buf

    char *inptr = inbuf;
    char *outptr = outbuf;

    // iconv_open
    iconv_t cd = iconv_open(toCode.c_str(), fromCode.c_str());
    if (cd == (iconv_t)-1) {
        printf("iconv_open failed: %s\n", strerror(errno));
        delete inbuf;
        delete outbuf;
        return std::string();
    }

    // iconv
    size_t res = iconv(cd, &inptr, &inbytes, &outptr, &outbytes);
    if (res == -1) {
        printf("iconv failed: %s\n", strerror(errno));
        iconv_close(cd);
        delete inbuf;
        delete outbuf;
        return std::string();
    }

    // iconv_close
    std::string outstr = outbuf;

    iconv_close(cd);
    delete inbuf;
    delete outbuf;

    return outstr;
}

}

static int readbuf(const std::string &file, char *buf, size_t bytes)
{
	int n = -1;
	int fd = open(file.c_str(), O_RDONLY);
	if (fd == -1) {
		printf("open '%s' failed: %s\n", file.c_str(), strerror(errno));
		return fd;
	}
	lseek(fd, 0L, SEEK_SET);
	if ((n = read(fd, buf, bytes)) < 0) {
		printf("%s readbuf failed: %s\n", file.c_str(), strerror(errno));
		fflush(NULL);
	}

	close(fd);
	return n;
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
	char buf[1024] = {0x00};
	readbuf("test_gbk.txt", buf, sizeof(buf));
	std::string gbkString = buf;
	printf("gbkString:\n%s\n", gbkString.c_str());
	std::string utfString = mpl::toUTF8(gbkString);
	printf("utf8String:\n%s\n", utfString.c_str());

	memset(buf, 0x00, sizeof(buf));
	readbuf("test_utf8.txt", buf, sizeof(buf));
	utfString = buf;
	printf("utf8String:\n%s\n", utfString.c_str());
	gbkString = mpl::toGBK(utfString);
	printf("gbkString:\n%s\n", gbkString.c_str());
    return 0;
}

// g++ -o main ticonv.cpp
