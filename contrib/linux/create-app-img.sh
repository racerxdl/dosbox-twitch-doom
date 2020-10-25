#!/bin/bash

set -x

# pip3 install --user appimage-builder
# https://github.com/linuxdeploy/linuxdeploy/releases/tag/continuous (meh...)

version=$(git describe --abbrev=5)
dest=$(pwd)/AppDir

./autogen.sh
./configure --prefix=/usr # CPPFLAGS=-DNDEBUG
make -j$(nproc)
make -C contrib/icons hicolor
make install DESTDIR="$dest"

install -DT -m 644 COPYING                              $dest/usr/share/doc/COPYING
install -DT -m 644 README                               $dest/usr/share/doc/README
install -DT -m 644 contrib/linux/dosbox-staging.desktop $dest/usr/share/applications/dosbox-staging.desktop

pushd contrib/icons/hicolor
install -p -m 0644 -Dt $dest/usr/share/icons/hicolor/16x16/apps    16x16/apps/dosbox-staging.png
install -p -m 0644 -Dt $dest/usr/share/icons/hicolor/22x22/apps    22x22/apps/dosbox-staging.png
install -p -m 0644 -Dt $dest/usr/share/icons/hicolor/24x24/apps    24x24/apps/dosbox-staging.png
install -p -m 0644 -Dt $dest/usr/share/icons/hicolor/32x32/apps    32x32/apps/dosbox-staging.png
install -p -m 0644 -Dt $dest/usr/share/icons/hicolor/scalable/apps scalable/apps/dosbox-staging.svg
popd

# MEH, it requires APT
# appimage-builder --generate
