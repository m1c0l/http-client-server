#include <sstream>
#include "HttpResponse.h"
using namespace std;

void HttpResponse::decodeFirstLine(string line) {
	stringstream ss;
	ss << line;

	string version, status, description;
	ss >> version >> status >> description;
	setVersion(version);
	setStatus(status);
	setDescription(description);
}

string HttpResponse::getStatus() {
	return m_status;
}

void HttpResponse::setStatus(string status) {
	m_status = status;
}

string HttpResponse::getDescription() {
	return m_statusDescription;
}

void HttpResponse::setDescription(string description) {
	m_statusDescription = description;
}

string HttpResponse::encodeFirstLine() {
	return getVersion() + " " + m_status + " " + m_statusDescription + "\r\n";
}
