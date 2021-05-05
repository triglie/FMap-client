#!/bin/bash

docker run --rm -it -v $PWD/pipeline/:/usr/share/logstash/pipeline/ -v $PWD/../logs/:/usr/share/logstash/logs docker.elastic.co/logstash/logstash:7.11.2
