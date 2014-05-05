#include "tempdir.bash"

cat >test.flo <<EOF
#include "SIFT-flo.bash"
EOF

ocn-neighbors --crossmesh 2 2 16 > network.ocn

#include "harness.bash"
