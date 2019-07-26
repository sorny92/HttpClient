//
// Created by esteve on 25/07/19.
//

#ifndef BEAST_TEST_HTTPCLIENT_H
#define BEAST_TEST_HTTPCLIENT_H

#include <string>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
namespace ssl = net::ssl;       // from <boost/asio/ssl.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class HttpClient {
public:
    HttpClient(const std::string &host, const std::string &port);

    http::response<http::dynamic_body> request(const std::string &target,
                                               const std::map<std::string, std::string> &values);

private:
    const std::string host;

    // It's a unique_ptr because the tcp_stream doesn't have a default initializer
    // so this solves the problem.
    std::unique_ptr<boost::beast::ssl_stream<boost::beast::tcp_stream>> stream;

    // The io_context is required for all I/O
    boost::beast::net::io_context ioc;

};


#endif //BEAST_TEST_HTTPCLIENT_H
