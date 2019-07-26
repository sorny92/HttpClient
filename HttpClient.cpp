//
// Created by esteve on 25/07/19.
//

#include <iostream>
#include <iomanip>
#include "HttpClient.h"


namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
namespace ssl = net::ssl;       // from <boost/asio/ssl.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


HttpClient::HttpClient(const std::string &host, const std::string &port) : host(host) {
    // The SSL context is required, and holds certificates
    boost::beast::net::ssl::context ctx(boost::asio::ssl::context::tlsv12_client);

    // Verify the remote server's certificate
    ctx.set_default_verify_paths();
    ctx.set_verify_mode(ssl::verify_peer);

    // These objects perform our I/O
    tcp::resolver resolver(ioc);
    stream = std::make_unique<boost::beast::ssl_stream<boost::beast::tcp_stream>>(ioc, ctx);

    // Set SNI Hostname (many hosts need this to handshake successfully)
    if (!SSL_set_tlsext_host_name(stream->native_handle(), host.c_str())) {
        beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
        throw beast::system_error{ec};
    }

    // Look up the domain name
    auto const results = resolver.resolve(host, port);

    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(*stream).connect(results);

    // Perform the SSL handshake
    stream->handshake(ssl::stream_base::client);
}

http::response<http::dynamic_body> HttpClient::request(const std::string &target, const std::map<std::string, std::string> &values) {

    std::stringstream ss;
    for (const auto &row: values) {
        ss << row.first << '=' << row.second << '&';
    }
// Set up an HTTP GET request message
    http::request<http::string_body> req;
    req.method(http::verb::post);
    req.set(http::field::host, host);
    // TODO: Http version hard coded
    req.version(11);
    req.target(target);
    req.set(http::field::content_type, "application/x-www-form-urlencoded");
    req.body() = ss.str();
    req.prepare_payload();

    // Send the HTTP request to the remote host
    http::write(*stream, req);

    // This buffer is used for reading and must be persisted
    beast::flat_buffer buffer;

    // Declare a container to hold the response
    http::response<http::dynamic_body> res;

    // Receive the HTTP response
    http::read(*stream, buffer, res);

    // Write the message to standard out
//    std::cout << res << std::endl;

    // Gracefully close the stream
    // TODO: This part does cabum when it is uncommented. Commented everything seems to work but this should be checked out
    beast::error_code ec;
//    stream->shutdown(ec);
    if (ec == net::error::eof) {
        // Rationale:
        // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
        ec = {};
    }
    if (ec) {
        throw beast::system_error{ec};
    }
    return res;
}