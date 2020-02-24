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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "gpioctl.h"

#define SYS_GPIO_DIR 	"/sys/class/gpio"
#define EXPORT_DIR		SYS_GPIO_DIR "/export"
#define UNEXPORT_DIR	SYS_GPIO_DIR "/unexport"
#define DIRECTION_DIR	SYS_GPIO_DIR "/gpio%d/direction"
#define VALUE_DIR		SYS_GPIO_DIR "/gpio%d/value"
#define EDGE_DIR		SYS_GPIO_DIR "/gpio%d/edge"

int gpio_export(int gpio)
{
	char gpio_num[8] = {0x00};
	int fd = open(EXPORT_DIR, O_WRONLY);
	if (fd < 0) {
		fprintf(stderr, "gpio: %d open %s: %s\n", gpio, EXPORT_DIR, strerror(errno));
		return -1;
	}
	snprintf(gpio_num, sizeof(gpio_num) - 1, "%d", gpio);
	int err = write(fd, gpio_num, strlen(gpio_num));
	close(fd);
	if (err < 0) {
		fprintf(stderr, "gpio: %d write %s: %s\n", gpio, EXPORT_DIR, strerror(errno));
		return -1;
	}
	return 0;
}

int gpio_unexport(int gpio)
{
	char gpio_num[8] = {0x00};
	int fd = open(UNEXPORT_DIR, O_WRONLY);
	if (fd < 0) {
		fprintf(stderr, "open %s: %s\n", UNEXPORT_DIR, strerror(errno));
		return -1;
	}
	snprintf(gpio_num, sizeof(gpio_num) - 1, "%d", gpio);
	int err = write(fd, gpio_num, strlen(gpio_num));
	close(fd);
	if (err < 0) {
		fprintf(stderr, "write %s: %s\n", UNEXPORT_DIR, strerror(errno));
		return -1;
	}
	return 0;
}

int gpio_direction_in(int gpio)
{
	char direction[64] = {0x00};
    snprintf(direction, sizeof(direction) - 1, DIRECTION_DIR, gpio);
    int fd = open(direction, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "open %s: %s\n", direction, strerror(errno));
		return -1;
	}
	int err = write(fd, "in", 2);
	close(fd);
	if (err < 0) {
		fprintf(stderr, "write 'in' to %s: %s\n", direction, strerror(errno));
		return -1;
	}
	return 0;
}

int gpio_direction_out(int gpio)
{
	char direction[64] = {0x00};
    snprintf(direction, sizeof(direction) - 1, DIRECTION_DIR, gpio);
    int fd = open(direction, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "open %s: %s\n", direction, strerror(errno));
		return -1;
	}
	int err = write(fd, "out", 3);
	close(fd);
	if (err < 0) {
		fprintf(stderr, "write 'out' to %s: %s\n", direction, strerror(errno));
		return -1;
	}
	return 0;
}

int gpio_value_set(int gpio, int value)
{
	char vpath[64] = {0x00};
	char vstr[8] = {0x00};
    snprintf(vpath, sizeof(vpath) - 1, VALUE_DIR, gpio);
    int fd = open(vpath, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "open %s: %s\n", vpath, strerror(errno));
		return -1;
	}
	snprintf(vstr, sizeof(vstr) - 1, "%d", value);
	int err = write(fd, vstr, strlen(vstr));
	close(fd);
	if (err < 0) {
		fprintf(stderr, "write '%d' to %s: %s\n", value, vpath, strerror(errno));
		return -1;
	}
	return 0;
}

int gpio_value_get(int gpio)
{
	char vpath[64] = {0x00};
	char vstr[8] = {0x00};
    snprintf(vpath, sizeof(vpath) - 1, VALUE_DIR, gpio);
    int fd = open(vpath, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "open %s: %s\n", vpath, strerror(errno));
		return -1;
	}
	int err = read(fd, vstr, sizeof(vstr));
	close(fd);
	if (err < 0) {
		fprintf(stderr, "read %s: %s\n", vpath, strerror(errno));
		return -1;
	}
	return atoi(vstr);
}

int gpio_set_edge(int gpio, const char *edge)
{
	int fd, len;
	char buf[64];

	len = snprintf(buf, sizeof(buf), EDGE_DIR, gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		fprintf(stderr, "set GPIO-%d edge: %s\n", gpio, strerror(errno));
		return fd;
	}

	write(fd, edge, strlen(edge) + 1);
	close(fd);
	return 0;
}

int gpio_fd_open(int gpio, int oflags)
{
	int fd, len;
	char buf[64];

	len = snprintf(buf, sizeof(buf), VALUE_DIR, gpio);

	fd = open(buf, oflags | O_NONBLOCK);
	if (fd < 0) {
		fprintf(stderr, "open %s: %s\n", buf, strerror(errno));
	}
	return fd;
}

int gpio_fd_close(int fd)
{
	return close(fd);
}

int gpio_setbit(int gpio)
{
	return gpio_value_set(gpio, 1);
}

int gpio_resetbit(int gpio)
{
	return gpio_value_set(gpio, 0);
}
