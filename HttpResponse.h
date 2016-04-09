#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include "HttpMessage.h"
using namespace std;

typedef unsigned short HttpStatus;

class HttpResponse : public HttpMessage {
private:
	HttpStatus m_status;
	string m_statusDescription;

public:
	virtual void decodeFirstLine(string line);
	HttpStatus getStatus();
	void setStatus(HttpStatus status);
	string getDescription();
	void setDescription(string description);
};

#endif
