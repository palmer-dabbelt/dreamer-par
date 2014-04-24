set -ex

tempdir=`mktemp -d -t ptest-dreamer-par.XXXXXXXXXX`
trap "rm -rf $tempdir" EXIT
cd $tempdir
