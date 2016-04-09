#include "httprequest.h"

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
