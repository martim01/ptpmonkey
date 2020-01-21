# ptpmonkey
This is a simple cross-platform library to implement a software only Precision Time Protocol client/analyser. 
It is not intended to do full hardware syncing of the client PC but to let the user know what clocks are on the system and give 
some idea of the of the offsets and delays on the network.
It can be used to sync client clocks, however as it is a software only implementation accuracy is an order of magnitude less than a full ptp client implementation.



## Prerequisites
* PTPMonkey is written in c++ and needs a c++14 compliant compiler.
* It uses the non-boost Asio libary for networking. This can be downloaded from [GiHhub](https://github.com/chriskohlhoff/asio) or [Think-Sync](http://think-async.com/Asio/).
Alternatively the CMake project will clone the GitHub files for you in to the relevant directories.

## Building
The simplest way to build the library is to use the CMake project.

```
cd {ptpmonkey directory}/build
cmake ..
cmake --build .
sudo cmake --build . --target install
```
This will check that the Asio library is downloaded to ``` {ptpmonkey directory}/external/asio ``` and build the libray.
It will then install it to ```/usr/local/lib/ ```

It is also possible to build the library using the provided Code::Blocks project.
