#include <DbusService.h>
#include <iostream>
#include <csignal>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>

Rgb::DbusService service;
atomic<bool> appExit;

void handler(const boost::system::error_code& error, int signal_number) {
  if (!error) {
    // A signal occurred.
	  appExit = true;
  }
}


int main()
{
	boost::asio::io_service ioService;
	boost::asio::signal_set signals(ioService, SIGINT, SIGTERM);
	boost::thread serviceThread(service);
	signals.async_wait(handler);
	while (!appExit) {
		ioService.run_one();
	}
	cout << "\nStopping...\n";
	serviceThread.interrupt();
	serviceThread.join();
	return 0;
}
