EXTRA_DIST		+= \
	debian/changelog.in \
	debian/changelog \
	debian/control

debian/changelog: debian/changelog.in
	$(top_builddir)/config.status --file $@:$<

MAINTAINERCLEANFILES	= debian/control debian/rules debian/changelog
