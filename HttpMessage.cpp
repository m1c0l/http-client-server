#include "HttpMessage.h"
#include <string>
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

string HttpMessage::getVersion(){
  return m_version;
}

void HttpMessage::setVersion(string ver){
  m_version = ver;
}

void HttpMessage::setHeader(string key, string value)
{
  m_headers[key] = value;
  return;
}

string HttpMessage::getHeader(string key)
{
  return m_headers[key];
}

void HttpMessage::decodeHeaderLine(string line)
//May need error checking if header line does not end with carriage return
{
  string key, value;
  int i = 0;
  i = line.find(":");

  key = line.substr(0, i);
  i++;
  while(line[i] == ' ')
    i++;
  value = line.substr(i, line.size());

  setHeader(key, value);
  return;
}

void HttpMessage::setPayLoad(string blob)
{
  m_payload = blob;
  return;
}

string HttpMessage::getPayload()
{
  return m_payload;
}

string HttpMessage::encode() {
	string msg = "";
	msg += encodeFirstLine();
	for (auto header : m_headers) {
		msg += header.first + ": " + header.second + "\r\n";
	}
	msg += "\r\n";
	msg += m_payload;
	return msg;
}

void HttpMessage::decode(string encoded) {
	const string CRLF = "\r\n";
	stringstream ss;
	ss << encoded;
	string line;

	size_t start = 0, end = 0;
	bool firstLine = true;

	// header
	while ((end = encoded.find(CRLF, start)) != string::npos) {
		line = encoded.substr(start, end - start);
		start = end + CRLF.size();

		if (line.size() == 0) // end of header
			break;

		if (firstLine)
			decodeFirstLine(line);
		else
			decodeHeaderLine(line);

		firstLine = false;
	}

	// body
	string length = getHeader("Content-Length");
	if (length.size() != 0 && stoi(length) != 0) {
		string blob = encoded.substr(start, stoi(length));
		setPayLoad(blob);
	}
}
