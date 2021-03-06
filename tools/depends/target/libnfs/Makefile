include ../../Makefile.include
DEPS= ../../Makefile.include Makefile

# lib name, version
LIBNAME=libnfs
VERSION=1.9.6
SOURCE=$(LIBNAME)-$(VERSION)
ARCHIVE=$(SOURCE).tar.gz

# configuration settings
CONFIGURE=CFLAGS="-DHAVE_NET_IF_H" ./configure --prefix=$(PREFIX)

LIBDYLIB=$(PLATFORM)/lib/.libs/$(LIBNAME).a

CLEAN_FILES=$(ARCHIVE) $(PLATFORM)

all: .installed-$(PLATFORM)

$(TARBALLS_LOCATION)/$(ARCHIVE):
	cd $(TARBALLS_LOCATION); $(RETRIEVE_TOOL) $(RETRIEVE_TOOL_FLAGS) $(BASE_URL)/$(ARCHIVE)

$(PLATFORM): $(TARBALLS_LOCATION)/$(ARCHIVE) $(DEPS)
	-rm -rf $(PLATFORM); mkdir -p $(PLATFORM)
	cd $(PLATFORM); $(ARCHIVE_TOOL) $(ARCHIVE_TOOL_FLAGS) $(TARBALLS_LOCATION)/$(ARCHIVE)
	cd $(PLATFORM); patch -p1 < ../readahead.patch
	sed -ie "s|utils||" "$(PLATFORM)/Makefile.am"
	cd $(PLATFORM); $(AUTORECONF) -vif
	cd $(PLATFORM); $(CONFIGURE)
ifeq ($(OS),ios)
	# this is nasty but cannot figure out how to get those flags out of libtool
	cd $(PLATFORM); patch -p1 < ../darwin-fixbitcode.patch
endif

$(LIBDYLIB): $(PLATFORM)
	$(MAKE) -C $(PLATFORM)

.installed-$(PLATFORM): $(LIBDYLIB)
	$(MAKE) -C $(PLATFORM) install
ifeq ($(OS),android)
	rm -f $(PREFIX)/lib/libnfs.la $(PREFIX)/lib/libnfs.so $(PREFIX)/lib/libnfs.so.4
	mv -f $(PREFIX)/lib/libnfs.so.4.3.0 $(PREFIX)/lib/libnfs.so
	$(RPL) -e "libnfs.so.4" "libnfs.so\x00\x00" $(PREFIX)/lib/libnfs.so
	-$(READELF) --dynamic $(PREFIX)/lib/libnfs.so | grep ibrary
endif
	touch $@
clean:
	$(MAKE) -C $(PLATFORM) clean
	rm -r .installed-$(PLATFORM)
	rm -rf $(PLATFORM)

distclean::
	rm -rf $(PLATFORM) .installed-$(PLATFORM)
