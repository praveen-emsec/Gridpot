FROM ubuntu:latest
RUN apt-get update && apt-get upgrade -y && apt-get install -y \
autoconf \
libtool \
subversion \
python3-dev \
mysql-server \
libmysqld-dev \
libxerces-c-dev \
python3-mysqldb \
python-pip \
libcurl4 \
libcurl4-openssl-dev \
python3-sphinx \
libxml2-dev \
libxslt1-dev \
doxygen \
libcppunit-dev \
libcppunit-doc \
libncurses5-dev \
libncursesw5-dev
RUN apt-get install libcurl4-gnutls-dev 
RUN apt-get install libmysqlclient-dev
RUN pip install -U sphinx lxml gevent python-dateutil mixbox pyasn1 pycryptodomex pysmi


RUN apt-get install -y git
RUN git clone https://github.com/sk4ld/Gridpott.git

WORKDIR /Gridpott/conpot/
RUN sudo make clean 
RUN sudo python setup.py install

RUN apt-get install cmake 
RUN apt-get install git
RUN apt-get install g++
WORKDIR /Gridpott
RUN git clone https://github.com/gridlab-d/gridlab-d.git
RUN cd gridlab-d
RUN git submodule update --init
RUN mkdir cmake_build
RUN cd cmake_build
RUN cmake ..
RUN make
RUN sudo make install


WORKDIR /Gridpott/libiec61850
RUN make
RUN sudo make INSTALL_PREFIX=/usr/local install


CMD ["sudo", "conpot", "-t", "gridpot"]

