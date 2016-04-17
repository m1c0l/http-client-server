#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include "HttpMessage.h"
using namespace std;

class HttpResponse : public HttpMessage {
private:
	string m_status;
	string m_statusDescription;

public:
	virtual void decodeFirstLine(string line);
	string getStatus();
	void setStatus(string status);
	string getDescription();
	void setDescription(string description);
	string encodeFirstLine();
};

#endif
