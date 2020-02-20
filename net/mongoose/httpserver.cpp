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
#include "httpserver.h"
#include "mongoose.h"

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

namespace web {

HttpServer::HttpServer()
	: _msTimeout(500)
	, _listDirectory(true)
	, _interrupt(false)
	, _init(false)
	, _nc(NULL)
{
	_mgr = new mg_mgr();
	mg_mgr_init(_mgr, NULL);

	_httpOpts = new struct mg_serve_http_opts();
}

HttpServer::~HttpServer()
{
	delete _httpOpts;
	mg_mgr_free(_mgr);
	delete _mgr;
}

int HttpServer::timeout() const { return _msTimeout; }
void HttpServer::setTimeout(int msec) { _msTimeout = msec; }
std::string HttpServer::address() const { return _addr; }
// void HttpServer::setAddress(const std::string &val) { _addr = val; }
std::string HttpServer::documentRoot() const { return _docRoot; }
// void HttpServer::setDocumentRoot(const std::string &doc) { _docRoot = doc; }
bool HttpServer::directoryListingEnabled() const { return _listDirectory; }
void HttpServer::setDirectoryListingEnabled(bool val) { _listDirectory = val; }

bool HttpServer::init(const std::string &addr, const std::string &docRoot)
{
	if (_init) return true;

	_addr = addr;
	_nc = mg_bind(_mgr, _addr.c_str(), HttpServer::onEventHandler, this);
	if (_nc == NULL) {
		printf("bind '%s' failed\n", _addr.c_str() );
		return false;
	}
	mg_set_protocol_http_websocket(_nc);
	printf("Starting web server on port: %s\n", _addr.c_str());

	_docRoot = docRoot;
	_httpOpts->document_root = _docRoot.c_str();

	_init = true;
	return true;
}

void HttpServer::onEventHandler(struct mg_connection *nc, int ev, void *ev_data, void *user_data)
{
    switch (ev) {
	case MG_EV_HTTP_REQUEST:
		struct http_message *msg = (struct http_message *) ev_data;
		HttpServer *srv = (HttpServer *)user_data;

		// receive message
		std::string message = std::string(msg->message.p, msg->message.len);
		printf("%s\n", message.c_str());

		srv->httpEventHandler(nc, msg);
		break;
    }
}

void HttpServer::start()
{
	if (!_init) return;
	
	_httpOpts->enable_directory_listing = _listDirectory ? "yes" : "no";

	while ( !isInterrupted() ) {
		mg_mgr_poll(_mgr, _msTimeout);
	}
}

void HttpServer::stop()
{
	interrupt();
}

bool HttpServer::compare(struct http_message *msg, const std::string &prefix)
{
	return (mg_vcmp(&(msg->uri), prefix.c_str()) == 0);
}

void HttpServer::httpEventHandler(struct mg_connection *nc, struct http_message *msg)
{
	if (!requestHandler(nc, msg))
		mg_serve_http(nc, msg, *_httpOpts);
}

bool HttpServer::requestHandler(struct mg_connection *nc, struct http_message *msg) {
	// override
    return false;
}

void HttpServer::sendHttp(struct mg_connection *nc, const std::string &buf)
{
	// Use chunked encoding in order to avoid calculating Content-Length
	mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

	// Output JSON object which holds BUF data
	mg_printf_http_chunk(nc, "%s", buf.c_str());

	// Send empty chunk, the end of response
	mg_send_http_chunk(nc, "", 0);
}

}

#ifdef _MSC_VER
# pragma warning (pop)
#endif
