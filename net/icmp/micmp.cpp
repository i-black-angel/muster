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
#include "micmp.h"
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

namespace mpl {

Icmp::Icmp() {}

Icmp::~Icmp() {
    memset(&dest_addr, 0, sizeof(dest_addr));
    sockfd = 0;
    pid = 0;
}

bool Icmp::ping(const std::string &ipAddr) {
    double rtt;
    struct hostent *host;
    struct protoent *protocol;
    int i, recv_status;
    unsigned long inaddr = 0l;

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;

    /*判断是主机名还是ip地址*/
    if ((inaddr = inet_addr(ipAddr.c_str())) == INADDR_NONE) {
        if ((host = gethostbyname(ipAddr.c_str())) == NULL) /*是主机名*/
        {
            perror("gethostbyname error");
            return false;
        }
        memcpy((char *)&dest_addr.sin_addr, host->h_addr, host->h_length);
    } else /*是ip地址*/
    {
        dest_addr.sin_addr.s_addr = inet_addr(ipAddr.c_str());
    }

    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) <
        0) { /* 创建原始ICMP套接字 */
        printf("[NetStatus]  error : socket");
        return false;
    }

    int iFlag;
    if (iFlag = fcntl(sockfd, F_GETFL, 0) < 0) {
        printf("[NetStatus]  error : fcntl(sockfd,F_GETFL,0)");
        _CloseSocket();
        return false;
    }
    iFlag |= O_NONBLOCK;
    if (iFlag = fcntl(sockfd, F_SETFL, iFlag) < 0) {
        printf("[NetStatus]  error : fcntl(sockfd,F_SETFL,iFlag )");
        _CloseSocket();
        return false;
    }

    pid = getpid();
    for (i = 0; i < MAX_NO_PACKETS; i++) {
        if (send_packet(i, sendpacket) < 0) {
            printf("[NetStatus]  error : send_packet");
            _CloseSocket();
            return false;
        }

        if (recv_packet(i, recvpacket) > 0) {
            _CloseSocket();
            return true;
        }
    }
    _CloseSocket();
    return false;
}

int Icmp::send_packet(int pkt_no, char *sendpacket) {
    int packetsize;
    packetsize = pack(pkt_no, sendpacket);
    gettimeofday(&tvsend, NULL);
    if (sendto(sockfd, sendpacket, packetsize, 0, (struct sockaddr *)&dest_addr,
               sizeof(dest_addr)) < 0) {
        printf("[NetStatus]  error : sendto error");
        return -1;
    }
    return 1;
}

int Icmp::pack(int pkt_no, char *sendpacket) {
    int i, packsize;
    struct icmp *icmp;
    struct timeval *tval;
    icmp = (struct icmp *)sendpacket;
    icmp->icmp_type = ICMP_ECHO;  //设置类型为ICMP请求报文
    icmp->icmp_code = 0;
    icmp->icmp_cksum = 0;
    icmp->icmp_seq = pkt_no;
    icmp->icmp_id = pid;            //设置当前进程ID为ICMP标示符
    packsize = ICMP_HEADSIZE + 56;  // sizeof(struct timeval);
    tval = (struct timeval *)icmp->icmp_data;
    gettimeofday(tval, NULL);
    icmp->icmp_cksum = cal_chksum((unsigned short *)icmp, packsize);
    return packsize;
}

unsigned short Icmp::cal_chksum(unsigned short *addr, int len) {
    int nleft = len;
    int sum = 0;
    unsigned short *w = addr;
    unsigned short answer = 0;
    while (nleft > 1)  //把ICMP报头二进制数据以2字节为单位累加起来
    {
        sum += *w++;
        nleft -= 2;
    }
    if (nleft ==
        1)  //若ICMP报头为奇数个字节,会剩下最后一字节.把最后一个字节视为一个2字节数据的高字节,这个2字节数据的低字节为0,继续累加
    {
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return answer;
}

int Icmp::recv_packet(int pkt_no, char *recvpacket) {
    int n, fromlen;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);
    // signal(SIGALRM,timeout);
    fromlen = sizeof(recv_addr);
    // alarm(MAX_WAIT_TIME);
    struct timeval seltime;

    seltime.tv_sec = 2;  // MaxSelectTimeSec;
    seltime.tv_usec = 0;
    // while(1)
    {
        select(sockfd + 1, &rfds, NULL, NULL, &seltime);
        if (FD_ISSET(sockfd, &rfds)) {
            // printf("recvfrom:\n");
            if ((n = recvfrom(sockfd, recvpacket, ICMP_PACKET_SIZE, 0,
                              (struct sockaddr *)&recv_addr,
                              (socklen_t *)&fromlen)) < 0) {
                if (errno == EINTR) return -1;
                perror("recvfrom error");
                return -2;
            }

            if (unpack(pkt_no, recvpacket, n) == -1) {
                return -1;
            }
        } else if (seltime.tv_sec == 0) {
            return -1;
        } else {
            // continue;
            return -1;
        }
        // gettimeofday(&tvrecv,NULL);

        return 1;
    }
}

int Icmp::unpack(int cur_seq, char *buf, int len) {
    int iphdrlen;
    struct ip *ip;
    struct icmp *icmp;
    ip = (struct ip *)buf;
    iphdrlen = ip->ip_hl << 2;  //求ip报头长度,即ip报头的长度标志乘4
    icmp = (struct icmp *)(buf + iphdrlen);  //越过ip报头,指向ICMP报头
    len -= iphdrlen;  // ICMP报头及ICMP数据报的总长度
    if (len < 8) return -1;

    // printf("icmp->icmp_type=%d, icmp->icmp_id=%d, icmp->icmp_seq=%d,
    // ICMP_ECHOREPLY=%d, ICMP_ECHO=%d\n", 	icmp->icmp_type, icmp->icmp_id,
    //icmp->icmp_seq, ICMP_ECHOREPLY, ICMP_ECHO);
    if ((icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id == pid) &&
        (icmp->icmp_seq == cur_seq)) {
        printf("%d byte from %s icmp_seq=%u ttl=%d\n", len,
               inet_ntoa(recv_addr.sin_addr), icmp->icmp_seq, ip->ip_ttl);
        return 0;
    } else {
        return -1;
    }
}

void Icmp::timeout(int signo) { printf("Request Timed Out\n"); }

void Icmp::tv_sub(struct timeval *out, struct timeval *in) {
    if ((out->tv_usec -= in->tv_usec) < 0) {
        --out->tv_sec;
        out->tv_usec += 1000000;
    }
    out->tv_sec -= in->tv_sec;
}

void Icmp::_CloseSocket() {
    close(sockfd);
    sockfd = 0;
}

}  // namespace mpl
