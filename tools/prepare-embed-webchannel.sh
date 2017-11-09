#!/bin/bash

# Update debian/ configuration to build with webchannel library.

sed -i -e '/webchannel/d' debian/control || exit 1
sed -i -e 's/EMBED_WEBCHANNEL.*/EMBED_WEBCHANNEL=ON \\/' debian/rules || exit 2
