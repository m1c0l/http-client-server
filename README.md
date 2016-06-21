# HTTP/1.0 Client and Server

Simple HTTP/1.0 client and server; works on the same machine or on the same network. The server serves files on the machine, and the client can download the server's files or download files from the Internet.

The server can handle multiple concurrent connections using multithreading, and sends data in chunks so that it can serve large files. It also supports status codes 200, 400, 404, 501, and 505. Both the server and client support timeouts.

`make` compiles the server and client, which can be run with `./web-server` and `./web-client`:

`./web-server [hostname] [port] [file-dir]`

`./web-client [URL] [URL]...`

The web client supports downloading multiple URLs, and can download from the server (from localhost) or from any website on the Internet, as long as you don't add URL parameters like ? and #.
