#include "HttpRequest.h"

#include <sstream>

string HttpRequest::getMethod() {
	return m_method;
}

void HttpRequest::setMethod(string method) {
	m_method = method;
}

string HttpRequest::getUrl() {
	return m_url;
}

void HttpRequest::setUrl(string url) {
	m_url = url;
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
	return m_method + " " +  m_url + " " + getVersion() + "\r\n";
}
