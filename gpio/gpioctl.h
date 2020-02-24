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
#ifndef _GPIOCTL_H_
#define _GPIOCTL_H_

#ifdef __cplusplus
extern "C" {
#endif

int gpio_export(int gpio);
int gpio_unexport(int gpio);
int gpio_direction_in(int gpio);
int gpio_direction_out(int gpio);
int gpio_value_set(int gpio, int value);
int gpio_value_get(int gpio);
int gpio_set_edge(int gpio, const char *edge);
int gpio_fd_open(int gpio, int oflags);
int gpio_fd_close(int fd);
int gpio_setbit(int gpio);
int gpio_resetbit(int gpio);
	
#ifdef __cplusplus
}
#endif

#endif /* _GPIOCTL_H_ */
