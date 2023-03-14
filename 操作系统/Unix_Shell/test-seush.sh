#! /bin/bash

if ! [[ -x seush ]]; then
    echo "seush executable does not exist"
    exit 1
fi

../tester/run-tests.sh $*


