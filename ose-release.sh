#!/bin/bash

HASH=$(openssl rand -hex 3)
COMMIT=$(git describe HEAD)
# removes the syslog-ng- prefix from the string, dpkg-source needs version strings starting with numbers
VERSION="${COMMIT#syslog-ng-}-$HASH"
echo "$VERSION" > VERSION
RELEASE="syslog-ng-$VERSION"
DISTTAR="$RELEASE.tar.gz"
DEBRELEASE="syslog-ng_$VERSION"
DSC="$DEBRELEASE.dsc"

ZBC_FILENAME="syslog-ng-$VERSION.zbc"

ZBS_INCOMING_DIR="build.syslog-ng:/var/etalon/zbs2/incoming"

./autogen.sh
rm -rf b/
mkdir b
cd b
../configure --enable-java --enable-debug --disable-mongodb --disable-amqp
make dist
tar -xvzf "$DISTTAR"
cd "$RELEASE"
dpkg-source -Us -Uc -b .
cd ..

cat << ZBC > $ZBC_FILENAME
Owner: btibi
Format: 1.0
Source-Format: debian
Snapshot: yes
Targets: linux-glibc2.11/syslog-ng-pe-5.6-ose-jviktor/amd64
Item: $DSC
ZBC

scp $DEBRELEASE.* $ZBS_INCOMING_DIR
scp $ZBC_FILENAME $ZBS_INCOMING_DIR
