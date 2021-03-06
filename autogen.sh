#!/bin/sh
#
# This script is mainly useful when testing autoconf/automake changes
# and as a part of their development process.

# If there's a Makefile, then run the 'distclean' target first (which
# will also remove the Makefile).
if test -f Makefile; then
  make distclean
fi

# Remove all tar-files (assuming there are some packages).
rm -f *.tar.* *.tgz

# Also remove the autotools cache directory.
rm -Rf autom4te.cache

# Remove rest of the generated files.
rm -f aclocal.m4 configure config.h* compile depcomp install-sh missing
find . -name Makefile.in -delete

# Generate configure and make files
aclocal
autoreconf --install
automake --add-missing --copy
autoconf
