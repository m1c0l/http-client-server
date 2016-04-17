#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H

#define CRLF "\r\n"

#include <string>
#include <unordered_map>

using namespace std;

class HttpMessage {
public:
	void setVersion(string ver);
	string getVersion();

	virtual void decodeFirstLine(string line) = 0;
	virtual string encodeFirstLine() = 0;

	void setHeader(string key, string value);
	string getHeader(string key);
	void decodeHeaderLine(string line);

	void setBody(string body);
	string getBody();

	void decode(string encoded);
	string encode();

private:
	string m_version;
	unordered_map<string, string> m_headers;
	string m_body;
};

#endif
