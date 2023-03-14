#! /bin/bash

if ! [[ -x seugrep ]]; then
    echo "seugrep executable does not exist"
    exit 1
fi

../../tester/run-tests.sh $*



