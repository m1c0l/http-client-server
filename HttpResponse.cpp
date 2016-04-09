#include "HttpResponse.h"
using namespace std;

void HttpResponse::decodeFirstLine(string line) {

}

HttpStatus HttpResponse::getStatus() {
	return m_status;
}

void HttpResponse::setStatus(HttpStatus status) {
	m_status = status;
}

string HttpResponse::getDescription() {
	return m_statusDescription;
}

void HttpResponse::setDescription(string description) {
	m_statusDescription = description;
}
