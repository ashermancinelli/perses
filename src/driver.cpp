#include <thread>
#include <chrono>
#include <cassert>
#include <NVMLWatcher.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
namespace ip = boost::asio::ip;

int main(int argc, char** argv) {
  std::vector<std::string> args(argv, argv+argc);

  const std::string HTTPOK_header = "HTTP/1.1 200 OK\nContent-Length: ";
  std::ostream& os = std::cout;
  bool useTCP = false;
  int port = 5045;
  int interval = 2;

  try {
    for (int i=1; i<args.size(); i++) {
      if (args[i] == "--gpulist") {
        assert(false && "GPUlist support is incomplete!");
      }
      else if (args[i] == "--help") {
        throw std::runtime_error("user passed --help flag");
      }
      else if (args[i] == "--interval") {
        if (++i == args.size())
          throw std::runtime_error(
              "Perses was passed option '--interval' with no interval specified!");
        interval = std::stoi(args[i]);
      }
      else if (args[i] == "--serve") {
        if (++i == args.size())
          throw std::runtime_error(
              "Perses was passed option '--serve' with no service specified!");
        if (args[i] == "local") {
          // keep os = std::cout
        }
        else if (args[i] == "tcp") {
          std::cout << "Serving via TCP\n";
          useTCP = true;
        }
      }
      else if (args[i] == "--port") {
        if (++i == args.size())
          throw std::runtime_error(
              "Perses was passed option '--port' with no port specified!");
        port = std::stoi(args[i]);
      }
    }
  }
  catch (std::exception& e) {
    std::cerr << "Perses caught exception \"" << e.what() << "\" when parsing arguments.\n\n";
    std::cerr << "Usage:\n\n\t./perses [--serve service_type] [--port port] [--gpulist gpulist] [--interval interval]\n\n"
      << "\tgpulist: a comma-separated list of GPUs to monitor, or keyword ALL. Default is ALL.\n\n"
      << "\tservice_type: either 'local' or 'tcp'. 'local' will print to standard out Default is 'local'.\n\n"
      << "\tport: an integer specifying the port to serve on. Ignored if service_type is local.\n\n"
      << "\tinterval: integer representing number of seconds in between socket accepts.\n\n";
    return 1;
  }

  try {
    boost::asio::io_service ioc;
    ip::tcp::endpoint endpoint(tcp::v4(), port);
    tcp::acceptor acceptor(ioc, endpoint);
    boost::system::error_code ec;

    NVMLWatcher w({});
    while (true) {
      if (useTCP) {
        tcp::socket socket(ioc);
        acceptor.accept(socket);
        tcp::iostream stream;
        if (ec) {
          throw std::runtime_error("error accepting socket");
        }
        std::cout << "Accepted new connection...\n";
        std::stringstream ss, message;
        ss << w;
        message << HTTPOK_header << ss.tellp() << "\n\n" << w;
        message.flush();
        boost::asio::write(socket, boost::asio::buffer(message.str()),
            boost::asio::transfer_all(), ec);
        if (ec) {
          throw std::runtime_error("error writing message to socket");
        }
      }
      else {
        os << w;
      }

      if (!useTCP)
        std::this_thread::sleep_for(std::chrono::seconds(interval));
    }
  }
  catch (std::exception& e) {
    std::cerr << "Perses caught exception \"" << e.what() << "\" in main loop.";
    return 1;
  }
  return 0;
}
