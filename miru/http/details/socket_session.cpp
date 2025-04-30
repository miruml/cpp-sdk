// std
#include <string>

// internal
#include <miru/http/errors.hpp>
#include <miru/http/details/socket_session.hpp>

// external
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast.hpp>

namespace miru::http {

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

std::string to_string(const RequestDetails& details) {
  return std::string(boost::beast::http::to_string(details.method)) + " " +
         details.url + " " + "(timeout: " + std::to_string(details.timeout.count()) +
         "ms) to socket: '" + details.socket_path + "'";
}

RequestDetails SocketSession::details() {
  return RequestDetails(req_.method(), socket_path_, req_.target(), timeout_);
}

// Start the asynchronous operation
void SocketSession::execute() {
  // Make the connection on the IP address we get from a lookup
  stream_.async_connect(
    stream_protocol::endpoint(socket_path_),
    beast::bind_front_handler(&SocketSession::on_connect, shared_from_this())
  );
}

void SocketSession::on_connect(beast::error_code ec) {
  if (ec) THROW_CONNECTION_ERROR(ec.what(), details());

  // Set a timeout on the operation
  stream_.expires_after(timeout_);

  // Send the HTTP request to the remote host
  http::async_write(
    stream_,
    req_,
    beast::bind_front_handler(&SocketSession::on_write, shared_from_this())
  );
}

void SocketSession::on_write(beast::error_code ec, std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  if (ec) THROW_WRITE_ERROR(ec.what(), details());

  // Set a timeout on the operation
  stream_.expires_after(timeout_);

  // Receive the HTTP response
  http::async_read(
    stream_,
    buffer_,
    res_,
    beast::bind_front_handler(&SocketSession::on_read, shared_from_this())
  );
}

void SocketSession::on_read(beast::error_code ec, std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  if (ec) THROW_READ_ERROR(ec.what(), details());

  // Gracefully close the socket
  stream_.socket().shutdown(tcp::socket::shutdown_both, ec);

  // not_connected happens sometimes so don't bother reporting it.
  if (ec && ec != beast::errc::not_connected)
    THROW_SHUTDOWN_ERROR(ec.what(), details());

  // if we get here then the connection is closed gracefully
}

}  // namespace miru::http
