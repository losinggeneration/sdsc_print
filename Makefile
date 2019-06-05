SUBDIRS = tools/smscheckhead sdcc wla-dx

all:
	for dir in ${SUBDIRS} ; do ( cd $$dir ; ${MAKE} all ) || exit 1 ; done

clean:
	for dir in ${SUBDIRS} ; do ( cd $$dir ; ${MAKE} clean ) ; done
