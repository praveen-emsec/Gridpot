Installation on host using Virtualenv
======================================

A generic way to keep Python installations separate is using `virtualenv <https://pypi.python.org/pypi/virtualenv>`_. This way you can run conpot on your machine without littering your machine. This guides assumes you have Python 3.6 installed and running on your computer.

Note that this is also the recommended way of installing conpot on a machine. Installation can be done as follows:-

Install dependencies:
::

    $ sudo apt-get install git libsmi2ldbl smistrip libxslt1-dev python3.6-dev libevent-dev default-libmysqlclient-dev

Create the virtualenv
::

    $ virtualenv --python=python3.6 conpot

Activate the environment
::

    $ source conpot/bin/activate

Upgrade any basic tools in the environment and deps
::

    $ pip install --upgrade pip
    $ pip install --upgrade setuptools
    $ pip install cffi

Install the table version of Conpot from PyPI:
::

    $ pip install conpot
