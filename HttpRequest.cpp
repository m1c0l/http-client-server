#include <string>
#include <sstream>
#include "HttpRequest.h"

using namespace std;

void HttpRequest::setMethod(string method) {
	m_method = method;
}

string HttpRequest::getMethod() {
	return m_method;
}

void HttpRequest::setUrl(string url) {
	m_url = url;
}

string HttpRequest::getUrl() {
	return m_url;
}

void HttpRequest::decodeFirstLine(string line) {
	stringstream ss;
	ss << line;
	string method, url, version;
	ss >> method >> url >> version;
	setMethod(method);
	setUrl(url);
	setVersion(version);
}

string HttpRequest::encodeFirstLine() {
	return m_method + " " + m_url + " " + getVersion() + CRLF;
}
