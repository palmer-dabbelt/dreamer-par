#include "tempdir.bash"

cat >test.flo <<EOF
#include "DREAMER-flo.bash"
EOF

ocn-neighbors --cmesh 4 4 4 > network.ocn

#include "harness.bash"
