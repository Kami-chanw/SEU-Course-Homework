#! /bin/bash

if ! [[ -x seucat ]]; then
    echo "seucat executable does not exist"
    exit 1
fi

../../tester/run-tests.sh $*


