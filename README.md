# Traffic accounting service

[![Build Status](https://travis-ci.org/agdsn/traffic-service-server.svg?branch=master)](https://travis-ci.org/agdsn/traffic-service-server)

This is a simple server to query traffic accounting data.

## Requirements

To compile this you need:

* Cmake (oldest tested version: 2.8.11.2) 
* Google Protobuf (minimum 2.6.0 because of the `oneof` keyword)
* ZeroMQ (oldest tested version: 4.0.4)
* Boost.Program_options (oldest tested version: 1.55.0.2)
* gcc (oldest tested version: 4.8 - you need C++11 support)

The current target is a Debian jessie (testing). The given versions
are tested versions. This does not mean that this are the only working
ones. To test your setup clone the project, compile it and run the
unittests.

## How to Clone

    $ git clone git@github.com:agdsn/traffic-service-server.git
    $ cd traffic-service-server
    $ git submodule init
    $ git submodule update
    
## How to Update (the source tree)

We use submodules within the project, so updating the source tree is 
not every time only a simple `git pull --rebase`. If a submodule have 
changed you ge something like:

    $ git status
    On branch master
    Your branch is up-to-date with 'origin/master'.
    Changes not staged for commit:
      (use "git add <file>..." to update what will be committed)
      (use "git checkout -- <file>..." to discard changes in working directory)
    
            modified:   messages/proto (new commits)
    
    no changes added to commit (use "git add" and/or "git commit -a")

Then you have to do a `git submodule update`. I recommend to read the 
[git submodule documentation](http://git-scm.com/book/de/Git-Tools-Submodule)
for better understanding what submodules mean and how to handle them.

## How to Build

First make all requirements available, then chdir into source root and:

    $ mkdir build
    $ cd build
    $ cmake .. -DCMAKE_BUILD_TYPE=Debug
    $ make

## Run Unittests

Within the `build` directory do:

    $ make all test

There is also a target to tun the tests under valgrind memcheck (you need
valgrind installed of course):

    $ make all test_memcheck

The last line of output from valgrind should contain:

    ERROR SUMMARY: 0 errors from 0 contexts

This means that there are no unsuppressed memory leaks (at least within the
unittests).

## Installation

Within the `build` directory do:

    $ make all install

You can choose the installation prefix by a Cmake variable called
`CMAKE_INSTALL_PREFIX:PATH`:

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

To find some work to do yust look at the issues of the project on
[github](https://github.com/agdsn/traffic-service-server).

## Common errors

### Missing submodules

If you have a version checked out from before a specific submodule was
used the new module will not automatically initializes with a
`git submodule update`. The error in this case is for example:

    CMake Error at test/CMakeLists.txt:4 (add_subdirectory):
      The source directory
    
        /home/jan/devel/traffic-service-server/test/gtest
    
      does not contain a CMakeLists.txt file.

To solve this you have to inizialize the missing submodule first by
executing `git submodule init` and then update it using `git submodule
update`. At this point I recommend again to read the 
[git submodule documentation](http://git-scm.com/book/de/Git-Tools-Submodule).

