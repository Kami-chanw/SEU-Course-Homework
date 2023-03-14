#! /bin/bash

if ! [[ -x seuzip ]]; then
    echo "seuzip executable does not exist"
    exit 1
fi

../../tester/run-tests.sh $*


