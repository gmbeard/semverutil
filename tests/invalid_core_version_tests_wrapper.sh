#!/usr/bin/env bash

./core_version_validity_tests --invalid <<-EOF

01.2.3
1.02.3
1.2.03
-1.2.3_4
1.-2.3_4
1.2.-3_4
1.2.3_-4
text
1.2 .3
EOF
