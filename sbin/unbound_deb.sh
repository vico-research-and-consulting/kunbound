#!/bin/bash

mkdir -p /opt/unbound/etc/unbound/dev && \
cp -a /dev/random /dev/urandom /dev/null /opt/unbound/etc/unbound/dev/

/usr/sbin/unbound -d
