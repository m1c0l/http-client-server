#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include "HttpMessage.h"

using namespace std;

class HttpRequest : public HttpMessage {
public:
	void setMethod(string method);
	string getMethod();

	void setUrl(string url);
	string getUrl();

	virtual void decodeFirstLine(string line);
	virtual string encodeFirstLine();

private:
	string m_url;
	string m_method;
};

#endif

