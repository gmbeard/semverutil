#!/usr/bin/env bash

./core_version_validity_tests <<-EOF
1.0.0
1.0.0_0
1.2.3_4
0.12.345_678
1.2.3
1
1.2
EOF
