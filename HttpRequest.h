#include <string>

#include "HttpMessage.h"

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

class HttpRequest : public HttpMessage {
public:
	string m_url;
	string m_method; // will only be GET; is this needed?
	string getMethod(); // ditto
	void setMethod(string method); // ditto
	string getUrl();
	void setUrl(string url);
	void decodeFirstLine(string line);
};

#endif

