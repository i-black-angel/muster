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
#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_

#include <mutex>
#include "mongoose.h"

struct mg_mgr;
struct mg_connection;
struct http_message;
struct mg_serve_http_opts;

namespace web {

class HttpServer
{
public:
    /*explicit */HttpServer();
    virtual ~HttpServer();
    bool init(const std::string &addr = "8080",
              const std::string &docRoot = "/var/www/html");
    void start();
    void stop();

	std::string address() const;
	std::string documentRoot() const;

	int timeout() const;
	void setTimeout(int msec);
	bool directoryListingEnabled() const;
	void setDirectoryListingEnabled(bool val);  /* Enabled by default. */

protected:	
	static void onEventHandler(struct mg_connection *nc, int ev, void *ev_data, void *user_data);
	
	bool isInterrupted() const { return _interrupt; }
	void interrupt() { std::lock_guard<std::mutex> locker(_mutex); _interrupt = true; }
	bool compare(struct http_message *msg, const std::string &prefix);
	void sendHttp(struct mg_connection *nc, const std::string &buf);
	
	struct mg_serve_http_opts *_httpOpts;
	void httpEventHandler(struct mg_connection *nc, struct http_message *msg);

	// override function
	virtual bool requestHandler(struct mg_connection *nc, struct http_message *msg);

private:
	std::mutex _mutex;
	bool _interrupt;
	bool _init;

	// Arguments
	int _msTimeout;	
	std::string _addr;
	std::string _docRoot;
	bool _listDirectory;

	struct mg_mgr *_mgr;
	struct mg_connection *_nc;	
};

}

#endif /* _HTTPSERVER_H_ */
