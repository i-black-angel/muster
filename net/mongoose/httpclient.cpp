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
#include <pthread.h>
#include <string>
#include <mongoose.h>

static int s_exit_flag = 0;
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data, void *user_data) {
  struct http_message *hm = (struct http_message *) ev_data;

  switch (ev) {
    case MG_EV_CONNECT:
      if (*(int *) ev_data != 0) {
        fprintf(stderr, "connect() failed: %s\n", strerror(*(int *) ev_data));
        s_exit_flag = 1;
      }
      break;
    case MG_EV_HTTP_REPLY:
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
    //   if (s_show_headers) {
        fwrite(hm->message.p, 1, hm->message.len, stdout);
    //   } else {
        // fwrite(hm->body.p, 1, hm->body.len, stdout);
    //   }
      putchar('\n');
      s_exit_flag = 1;
      break;
    case MG_EV_CLOSE:
      if (s_exit_flag == 0) {
        printf("Server closed connection\n");
        s_exit_flag = 1;
      }
      break;
    default:
      break;
  }
}

int main(int argc, char *argv[]) {        
    struct mg_mgr mgr;
    mg_mgr_init(&mgr, NULL);

    const char *url = "http://localhost:8080";
    struct mg_connection *nc = mg_connect_http(&mgr, ev_handler, NULL, url, NULL, NULL);
    if (NULL == nc) {
        printf("Failed to connect server\n");
        return 1;
    }

    while (s_exit_flag == 0) {
        mg_mgr_poll(&mgr, 100);
    }
    mg_mgr_free(&mgr);
    return 0;
}
