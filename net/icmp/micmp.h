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
#ifndef _MICMP_H_
#define _MICMP_H_

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

namespace mpl {
class Icmp {
public:
    Icmp();
    ~Icmp();

    bool ping(const std::string &ipAddr);

private:
	static const int ICMP_PACKET_SIZE = 640;
    static const int ICMP_HEADSIZE    = 8;
    static const int MAX_NO_PACKETS   = 2;
    static const int MAX_WAIT_TIME    = 1;
	
    struct sockaddr_in dest_addr;
    struct sockaddr_in recv_addr;
    struct timeval tvsend;
    struct timeval tvrecv;
    int sockfd;
    pid_t pid;
    char sendpacket[ICMP_PACKET_SIZE];
    char recvpacket[ICMP_PACKET_SIZE];

    void timeout(int signo);
    unsigned short cal_chksum(unsigned short *addr, int len);
    int pack(int pkt_no, char *sendpacket);
    int send_packet(int pkt_no, char *sendpacket);
    int recv_packet(int pkt_no, char *recvpacket);
    int unpack(int cur_seq, char *buf, int len);
    void tv_sub(struct timeval *out, struct timeval *in);
    void _CloseSocket();
};
}  // namespace mpl

#endif /* _MICMP_H_ */
