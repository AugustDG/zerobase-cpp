# zerobase-cpp

## Welcome

Hey! Welcome to ZeroBase, a pub-sub communications framework based on [ZeroMQ](https://zeromq.org/) for multiprocessing systems!

This is a C++ library that provides a base class for building ZeroMQ & pub-sub -based programs for NanoStride. It handles all of the necessary setup and teardown for ZeroMQ, and provides a simple interface for sending and receiving messages. The library is designed to be easy to use and flexible, allowing you to quickly build and deploy ZeroMQ-based programs for a variety of use cases.

With `ZeroBase`, you can focus on building the application logic, while the library takes care of the underlying ZeroMQ infrastructure.

That being said, it is still a work in progress, so don't hesitate to create an issue or start a PR!

Yours truly, <br>
Augusto M.P

## Getting Started

Below you'll find the necessary information to install and get started with `zerobase`. For in-depth documentation, check out the [docs](https://github.com/Nanostride/zerobase-python/blob/master/docs/index.md) folder in the `zerobase-python` repo. Eventually, the docs will be moved to a central location.

### Installation

#### CMake

1. Clone the repo (as a submodule or otherwise) into the `vendor` directory.
2. Add the following to your `CMakeLists.txt` file (of course, this can be modified to suit your needs):

```cmake
add_subdirectory(vendor/zerobase-cpp)
target_link_libraries(<your-target> zerobase)
```
3. Run `cmake` and your build system of choice.
4. Include the `zerobase.h` header in your source files!

### Quickstart

Once you have the library, take a look at [`examples/basic.cpp`](examples/basic.cpp) for a basic example on how to use the library.

The main function generates random messages and sends them on example topics "A", "B", and "C". The `MsgReceived` function is called whenever a message is received on one of the subscribed topics, and simply prints out the message and topic.

To customize the code for your own use case, you can modify the main function to generate and send messages on different topics, and modify the `MsgReceived` function to handle incoming messages according to the given topic. You can also modify the passed vectors for publisher and subscriber config objects to change the addresses and topics used by the publisher and subscribers.

## License

This project is licensed under the GPLv3 license - see the [LICENSE](LICENSE) file for details.
