#include <thread>
#include <chrono>
#include <cassert>
#include <NVMLWatcher.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
namespace ip = boost::asio::ip;

static bool useTCP = false, silent = false;
static int port = 5045;
static int interval = 2;
static const std::string HTTPOK_header = "HTTP/1.1 200 OK\nContent-Length: ";

void serve(NVMLWatcher& w) {
  static boost::asio::io_service ioc;
  static ip::tcp::endpoint endpoint(tcp::v4(), port);
  static tcp::acceptor acceptor(ioc, endpoint);
  static boost::system::error_code ec;

  tcp::socket socket(ioc);
  acceptor.accept(socket);
  tcp::iostream stream;
  if (ec) {
    throw std::runtime_error("error accepting socket");
  }
  if (!silent)
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
  if (!silent)
    std::cout << "Successfully wrote the following message to client:\n\n"
      << w << "\n";
}

int main(int argc, char** argv) {
  std::vector<std::string> args(argv, argv+argc);
  std::ostream& os = std::cout;

  try {
    for (int i=1; i<args.size(); i++) {
      if (args[i] == "--gpulist") {
        assert(false && "GPUlist support is incomplete!");
      }
      else if (args[i] == "--silent") {
        silent = true;
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
          // do nothing for now
        }
        else if (args[i] == "tcp") {
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
    std::cerr << "Perses caught exception \"" << e.what() 
      << "\" when parsing arguments.\n\n"
      << "Usage:\n\n\t./perses [--silent] [--serve service_type]"
      << " [--port port] [--gpulist gpulist] [--interval interval]\n\n"
      << "\tgpulist: a comma-separated list of GPUs to monitor, or keyword ALL. Default is ALL.\n"
      << "\tservice_type: either 'local' or 'tcp'. 'local' will print to standard out. Default is 'local'.\n"
      << "\tport: an integer specifying the port to serve on. Ignored if service_type is local.\n"
      << "\tinterval: integer representing number of seconds in between socket accepts.\n"
      << "\t--silent: supresses output when running server.\n\n";
    return 1;
  }

  try {

    NVMLWatcher w({});
    while (true) {
      if (useTCP) {
        serve(w);
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
