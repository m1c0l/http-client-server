#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "HttpMessage.h"

using namespace std;

void HttpMessage::setVersion(string ver) {
	m_version = ver;
}

string HttpMessage::getVersion(){
	return m_version;
}

void HttpMessage::setHeader(string key, string value)
{
	m_headers[key] = value;
}

string HttpMessage::getHeader(string key)
{
	return m_headers[key];
}

int HttpMessage::decodeHeaderLine(string line)
{
	string key, value;
	size_t i = 0;
	i = line.find(":");

	if (i == string::npos) {
		return 400;
	}

	key = line.substr(0, i);
	i++;
	while(line[i] == ' ')
		i++;
	value = line.substr(i, line.size());

	setHeader(key, value);
	return 0;
}

int HttpMessage::decode(string encoded) {
	stringstream ss;
	ss << encoded;
	string line;

	size_t start = 0, end = 0;
	bool firstLine = true;

	// header
	while ((end = encoded.find(CRLF, start)) != string::npos) {
		line = encoded.substr(start, end - start);
		start = end + sizeof(CRLF) - 1;

		if (line.size() == 0 && !firstLine) // end of header
			break;

		if (firstLine) {
			int decodeStatus = decodeFirstLine(line);
			if (decodeStatus) {
				return decodeStatus;
			}
		}
		else {
			int decodeStatus = decodeHeaderLine(line);
			if (decodeStatus) {
				return decodeStatus;
			}
		}

		firstLine = false;
	}

	return 0;
}

string HttpMessage::encode() {
	string msg = "";
	msg += encodeFirstLine();
	for (auto header : m_headers) {
		msg += header.first + ": " + header.second + CRLF;
	}
	msg += CRLF;
	return msg;
}
