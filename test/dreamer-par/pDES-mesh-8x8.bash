#include "tempdir.bash"

cat >test.flo <<"EOF"
#include "pDES-flo-mesh.bash"
EOF

ocn-neighbors --mesh 8 8 > network.ocn

#include "harness.bash"
