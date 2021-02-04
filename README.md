# Perses

Prometheus-esc monitoring tool for NVIDIA GPU devices.

## Installation

Perses uses a typical CMake workflow, and depends on CUDA (of course), Boost, and c++17.

Example:

```console
$ git clone https://github.com/ashermancinelli/perses.git
$ cd perses
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./build/src/perses --help
Perses caught exception "user passed --help flag" when parsing arguments.

Usage:

	./perses [--silent] [--serve service_type] [--port port] [--gpulist gpulist] [--interval interval]

	gpulist: a comma-separated list of GPUs to monitor, or keyword ALL. Default is ALL.
	service_type: either 'local' or 'tcp'. 'local' will print to standard out. Default is 'local'.
	port: an integer specifying the port to serve on. Ignored if service_type is local.
	interval: integer representing number of seconds in between socket accepts.
	--silent: supresses output when running server.
```

## Example Uses

This is used as a process to collect GPU performance data from clusters. In this case, you most likely want to serve on a port like so:

```console
$ ./perses --serve tcp --port 5045 --silent
```

This will write the data read off of the GPUs available on the machine to a local socket on port 5045.
The `--silent` flag supresses out put from the server, which logs all messages sent to clients.


