#include "tempdir.bash"

cat >test.flo <<EOF
#include "DES-flo.bash"
EOF

ocn-neighbors --cmesh 2 2 16 > network.ocn

#include "harness.bash"
