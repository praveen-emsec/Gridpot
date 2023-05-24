#ARG DEBIAN_FRONTEND=noninteractive

FROM ubuntu:20.04


RUN echo "deb http://dk.archive.ubuntu.com/ubuntu focal main multiverse" >>  /etc/apt/sources.list

#RUN echo "deb http://archive.ubuntu.com/ubuntu $(lsb_release -sc) main universe multiverse restricted" >> /etc/apt/sources.list

RUN apt-get update

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y libmysqlclient-dev libsmi2ldbl snmp-mibs-downloader python3-dev libevent-dev \
libxslt1-dev libxml2-dev python3-pip python3-mysqldb pkg-config libvirt-dev  python3-setuptools 

RUN apt install -y  git

#RUN git --version

RUN pip3 install stix-validator && pip3 install cybox

RUN git clone https://github.com/mujeebch/conpot.git

WORKDIR conpot

#RUN ls conpot/
RUN pip3 install -r requirements.txt


RUN python3 setup.py install

RUN chmod -R a+rw /usr/local/lib/python3.8/dist-packages/conpot-0.6.0-py3.8.egg/conpot/tests/data/data_temp_fs/



RUN apt-get install authbind


#RUN touch /etc/authbind/byport/80
#root@lappy:~# chown skx:skx /etc/authbind/byport/80
#RUN chmod 555 /etc/authbind/byport/80

#RUN touch /etc/authbind/byport/502
#RUN chmod 555 /etc/authbind/byport/502

#RUN touch /etc/authbind/byport/102
#RUN chmod 555 /etc/authbind/byport/102

#CMD ["/home/conpot/.local/bin/conpot", "--template", "default", "--logfile", "/var/log/conpot/conpot.log", "-f", "--temp_dir", "/tmp" ]


CMD ["authbind","conpot","-f","-t","guardian_ast","--temp_dir","/tmp"]
