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

int HttpRequest::decodeFirstLine(string line) {
	stringstream ss;
	ss << line;
	string method, url, version;
	ss >> method >> url >> version;
	if (method != "GET") {
		// only GET supported
		return 501;
	}
	if (version == "HTTP/1.1") {
		// this HTTP version not implemented yet
		return 505;
	}
	else if (version != "HTTP/1.0") {
		// bad version string
		return 400;
	}
	if (url.substr(0,7) == "http://"){
	  url = url.substr(7);
	}
	//size_t colonPos = url.find_first_of(":");
	size_t slashPos = url.find_first_of("/");
	/*
	string hostName = url;
	if (colonPos != string::npos) {
	  hostname = url.substr(0,colPos);
	}
	else if (slashPos != string::npos) {
	  hostname = url.substr(0,slashPos);
	}

	string port = "80";
	if (colonPos != string::npos && slashPos != string::npos) {
	  port = url.substr(colonPos + 1, slashPos - colonPos - 1);
	}
	else if (colonPos != string::npos) {
	  port = url.substr(colonPos + 1);
	}
	*/
	string filename = "/";
	if (slashPos != string::npos) {
	  filename = url.substr(slashPos);
	}
	setMethod(method);
	setUrl(filename);
	setVersion(version);
	return 0;
}

string HttpRequest::encodeFirstLine() {
	return m_method + " " + m_url + " " + getVersion() + CRLF;
}
