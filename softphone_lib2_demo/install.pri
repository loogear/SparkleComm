INSTALL_DIR =../install-bin/
win32 {
    build_pass: CONFIG(debug, debug|release) {
        DESTDIR_BUILD = $$OUT_PWD/debug/
    }
    else: build_pass {
        DESTDIR_BUILD = $$OUT_PWD/release/
    }
 DESTDIR ~= s,/,\\,g
 INSTALL_DIR ~= s,/,\\,g
 PREFIX =.dll
 SUFIX =
}

ios {
    SUFIX=lib
    PREFIX =.a
    DESTDIR = $$INSTALL_DIR/
}

macx {
   # SUFIX=lib
   # PREFIX =.dylib
    DESTDIR = $$INSTALL_DIR/
}


INSTALL_PATH = $$DESTDIR
# Shadow build detection
#!equals(PWD, $$DESTDIR) INSTALL_PATH = $$replace(INSTALL_PATH, build, install)

# Loop over all given directories and append the 'install' directory to make absolute paths
for(DIR, DIRS) ABS_DIRS += $$DIR

EXTRA_BINFILES +=$$DESTDIR_BUILD$$SUFIX$$TARGET$$PREFIX
win32{
LIB_PREFIX=.lib
EXTRA_BINFILES +=$$DESTDIR$$TARGET$$LIB_PREFIX
}
# Create 'copy' commands for $DIRS
for(FILE, EXTRA_BINFILES) CP_CMD += $(COPY) $$FILE $$INSTALL_DIR $$escape_expand(\n\t)

#create.commands = $(MKDIR) $$INSTALL_DIR

QMAKE_EXTRA_TARGETS += create
POST_TARGETDEPS += create
win32{
QMAKE_POST_LINK += $$CP_CMD
}
macx{
#QMAKE_POST_LINK += $$CP_CMD

}
