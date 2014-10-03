# Traffic accounting service

This is a simple server to query traffic accounting data.

## Requirements

To compile this you need:

* Cmake
* Google Protobuf
* ZeroMQ
* Boost.Program_options

## How to build

First make all requirements available, then:

    $ git clone git@github.com:agdsn/traffic-service-server.git
    $ cd traffic-service-server
    $ git submodule init
    $ git submodule update
    
    $ mkdir build
    $ cd build
    $ cmake .. -DCMAKE_BUILD_TYPE=Debug
    $ make

## Run Unittests

Within the `build` directory do:

    $ make all test

## Installation

Within the `build` directory do:

    $ make all install

You can choose the installation prefix by a Cmake variable
called `CMAKE_INSTALL_PREFIX:PATH`:

    $ cmake -DCMAKE_INSTALL_PREFIX:PATH=../run

## Commandline options

    -h [ --help ]               Print help message.
    -b [ --bind ] arg (=*:3444) address:port combinations to bind to
    -w [ --worker ] arg (=2)    Number of workers to spawn
    -s [ --storage ] arg        Specify the backend storage type (sqlite, mysql, 
                                postgres)
    -I [ --table_in ] arg       The database table name for incomming traffic
    -O [ --table_out ] arg      The database table name for outgoing traffic


## Contributing

As long as this is developed by a single person (me) I will push
directly to this repository. When someone want to join, we should
clone our personal copies and synchronize via pull-requensts.
