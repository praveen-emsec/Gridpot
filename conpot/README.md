# Conpot

[![Build Status](https://travis-ci.org/mushorg/conpot.svg?branch=master)](https://travis-ci.org/mushorg/conpot)
<!--- [![Code Health](https://landscape.io/github/mushorg/conpot/master/landscape.png)](https://landscape.io/github/mushorg/conpot/master)  --->
[![Python Version](https://img.shields.io/pypi/pyversions/conpot.svg)](https://pypi.python.org/pypi/Conpot) 
[![PyPI version](https://badge.fury.io/py/Conpot.svg)](https://badge.fury.io/py/Conpot)
[![Docs](https://readthedocs.org/projects/conpot/badge/?version=latest)](https://conpot.readthedocs.io/en/latest/)
[![Coverage Status](https://coveralls.io/repos/github/mushorg/conpot/badge.svg?branch=master)](https://coveralls.io/github/mushorg/conpot?branch=master)
[![Docker Build Status](https://img.shields.io/docker/build/honeynet/conpot.svg)](https://hub.docker.com/r/honeynet/conpot)
[![Docket Image Size](https://img.shields.io/microbadger/image-size/honeynet/conpot.svg)](https://hub.docker.com/r/honeynet/conpot)
[![Docker Pulls](https://img.shields.io/docker/pulls/honeynet/conpot.svg)](https://hub.docker.com/r/honeynet/conpot)

## About

Conpot is an ICS honeypot with the goal to collect intelligence about the motives and
methods of adversaries targeting industrial control systems

## Documentation

The documentation can be found [here](https://conpot.readthedocs.io/). If you are just checking out conpot, we
suggest that you go for [quick install](https://conpot.readthedocs.io/en/latest/installation/quick_install.html). 

If you want to tinker around and write your own template, change ports etc. We suggest that you do host install.
You can find instructions on how to install conpot [here](https://conpot.readthedocs.io/en/latest/installation/install.html) and the FAQ [here](https://conpot.readthedocs.io/en/latest/faq.html).

## Changes made by Mujeeb

* We have used the default, IEC104, Guardian_AST, Kamstrup_382 templates and customised those as well.
* most modifications are done on default template in an attempt to make it look like more real device rather than a honeypot.
* For the rest of the templates we have modified the port numbers to represent the well known ports for each protocol. 
* we have added the docker-compose and Docker file for all of the new modifications in a new folder called "Customized Dockerfiles"

## Few challenges in Installation of conpot

* Anyone wants to install on the native Linux OS without running a docker, can use our Dockerfile and follow the steps in there to install on their system. 
* Use python3 and pip3 for all the development.
* specify the temporary directory in the command for running conpot as "--temp_dir"
* install and use authbind for the ports < 1024
* use authbind in CMD while running conpot or in the shell when running from the computer used for conpot
