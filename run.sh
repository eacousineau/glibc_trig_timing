#!/bin/bash
set -e -u

cd "$(dirname "$BASH_SOURCE")"

mkdir -p build
(
    cd build
    cmake ..
    make
)

good_values="$(
python <<EOF
from numpy import pi, linspace
values = linspace(-1, 1, 11).tolist() + linspace(-pi, pi + 0.01, 13).tolist()
print ' '.join(map(str, values))
EOF
)"

bad_values="
    0.93340582292648832662962377071381
    2.3328432680770916363144351635128
    3.7439477503636453548097051680088
    3.9225160069792437411706487182528
    4.0711651639931289992091478779912
    4.7858438478542097982426639646292
    "

indent()
{
    sed 's#^#    #g'
}

run-test()
{
    echo "-   func: $1"
    {
        echo "good:"
        for value in $good_values; do
            sudo ./build/sintest "$@" $value
        done
        echo "bad:"
        for value in $bad_values; do
            sudo ./build/sintest "$@" $value
        done
    } | indent
}

sudo echo "Initialize sudo permissions"

mkdir -p data

{
    run-test sin
    run-test sinf
    run-test sinl
} | tee data/results.yaml
