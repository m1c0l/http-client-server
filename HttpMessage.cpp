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

void HttpMessage::setBody(string body)
{
	m_body = body;
}

string HttpMessage::getBody()
{
	return m_body;
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

		if (line.size() == 0) // end of header
			break;

		if (firstLine) {
			if (decodeFirstLine(line) == 400) {
				return 400;
			}
		}
		else {
			if (decodeHeaderLine(line) == 400) {
				return 400;
			}
		}

		firstLine = false;
	}

	// body
	string length = getHeader("Content-Length");
	if (length.size() != 0 && stoi(length) != 0) {
		string body = encoded.substr(start, stoi(length));
		setBody(body);
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
	msg += m_body;
	return msg;
}
