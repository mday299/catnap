#ifndef __LIBCATNAP_HTTPHANDLER_H__
#define __LIBCATNAP_HTTPHANDLER_H__

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>

#include <string>

/**
 * Create a TCP acceptor and create an async listener.
 *
 * @param [in, out] ioc
 *      IO Context required by ASIO to do it's magic.
 * @param [in] doc_root
 *      Document root for serving documents.
 * @param [in] address
 *      Address to listen on, eg. 127.0.0.1, 0.0.0.0...
 * @param [in] port
 *      Port to bind to.
 *
 * @return
 *      Returns (true) if the operation was a success.
 */
bool setup_tcp_acceptor(boost::asio::io_context &ioc,
        const std::shared_ptr<std::string> doc_root,
        boost::asio::ip::address address, unsigned short port);

/**
 * Create a UNIX acceptor and create an async listener.
 *
 * @param [in, out] ioc
 *      IO Context required by ASIO to do it's magic.
 * @param [in] doc_root
 *      Document root for serving documents.
 * @param [in] path
 *      Socket path, this will create a socket file. If anonymous, a null will
 *      be prepended and the path will be used as the symbolic name used by the
 *      os; no file will be created.
 * @param [in] is_anonymous
 *      (true) to create an anonymous UNIX socket instead of one that creates a
 *      socket file.
 *
 * @return
 *      Returns (true) if the operation was a success.
 */
bool setup_unix_acceptor(boost::asio::io_context &ioc,
        const std::shared_ptr<std::string> doc_root,
        std::string path, bool is_anonymous = false);

#endif /* __LIBCATNAP_HTTPHANDLER_H__ */
