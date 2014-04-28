#include "tempdir.bash"

cat >test.flo <<"EOF"
#include "pDES-flo.bash"
EOF

ocn-neighbors --crossbar 64 > network.ocn

#include "harness.bash"
