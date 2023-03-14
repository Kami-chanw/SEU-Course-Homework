#! /bin/bash

if ! [[ -x seuunzip ]]; then
    echo "seuunzip executable does not exist"
    exit 1
fi

../../tester/run-tests.sh $*


