# Traffic accounting service

This is a simple server to query traffic accounting data.

## Requirements

To compile this you need:

* Cmake
* Google Protobuf
* ZeroMQ
* Boost.Program_options

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
