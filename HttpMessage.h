#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H
#include <string>
#include <cstdlib>
#include <map>

using namespace std;

class HttpMessage {
public:
	string getVersion();
	void setVersion(string ver);
	void setHeader(string key, string value);
	string getHeader(string key);
	void decodeHeaderLine(string line);
	void setPayLoad(string blob);
	string getPayload();
	virtual void decodeFirstLine(string line) = 0;
	virtual string encodeFirstLine() = 0;
	string encode();

private:
	string m_version;
	map<string, string> m_headers;
	string m_payload;
};

#endif
