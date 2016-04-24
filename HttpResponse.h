#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include "HttpMessage.h"

using namespace std;

class HttpResponse : public HttpMessage {
public:
	void setStatus(string status);
	string getStatus();

	void setDescription(string description);
	string getDescription();

private:
	string m_status;
	string m_statusDescription;

	virtual void decodeFirstLine(string line);
	virtual string encodeFirstLine();
};

#endif
