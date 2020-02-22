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
#include <string>
#include <mongoose.h>

static struct mg_serve_http_opts s_http_server_opts;

static void ev_handler(struct mg_connection *nc, int ev, void *p, void *user_data) {
    if (MG_EV_HTTP_REQUEST == ev) {
        struct http_message *msg = (struct http_message *)p;
        std::string msg_request(msg->message.p, msg->message.len);
        printf("%s\n", msg_request.c_str());

        mg_serve_http(nc, (struct http_message *)p, s_http_server_opts);
    }
}

int main(int argc, char *argv[]) {
    struct mg_mgr mgr;
    struct mg_connection *nc;

    // init mgr
    mg_mgr_init(&mgr, NULL);
    nc = mg_bind(&mgr, "8080", ev_handler, NULL);
    printf("Starting web server on port 8080\n");
    // Set up HTTP server parameters
    mg_set_protocol_http_websocket(nc);
    s_http_server_opts.document_root = ".";  // Serve current directory
    s_http_server_opts.enable_directory_listing = "yes";

    while (true) {
        mg_mgr_poll(&mgr, 100);
    }

    mg_mgr_free(&mgr);
    return 0;
}
