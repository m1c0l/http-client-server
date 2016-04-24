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

	void setHeader(string key, string value);
	string getHeader(string key);

	void setBody(string body);
	string getBody();

	int decode(string encoded);
	string encode();

private:
	string m_version;
	unordered_map<string, string> m_headers;
	string m_body;

	virtual int decodeFirstLine(string line) = 0;
	virtual string encodeFirstLine() = 0;
	int decodeHeaderLine(string line);
};

#endif
