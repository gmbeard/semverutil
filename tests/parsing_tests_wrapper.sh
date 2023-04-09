#!/usr/bin/env bash

./parsing_tests ${@} <<-EOF
1.0.0 0.1.0
1.2.3-prerelease
EOF
