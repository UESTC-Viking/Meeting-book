#python2.7
# -*- coding:utf-8 -*-
from BaseHTTPServer import HTTPServer, BaseHTTPRequestHandler
 
HOST = "127.0.0.1"
PORT = 80
 
class RequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
    	print "[New Verify Request]"
        self.send_response(200)
        self.send_header("Content-Type", "text/html")
        self.end_headers()
        self.wfile.write("<ss>valid</ss><id>1</id>".encode("utf-8"))
 
if __name__ == "__main__":
    print "010editor http server run..."
    try:
        server = HTTPServer((HOST,PORT),RequestHandler)
        server.serve_forever()
    except KeyboardInterrupt:
        print '^c received,shutting down the web server!'
        server.socket.close()