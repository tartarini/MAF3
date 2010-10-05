# import utility foundation library references

#copy dynamic libraries into Install/bin

####### Windows #######
win32 {
    !exists( $$DESTDIR\qxmlrpc.dll ):QMAKE_PRE_LINK += copy \
    $$(MAF3_FOUNDATION_LIB_DIR)\qxmlrpc\lib\*qxmlrpc* \
    $$DESTDIR \
    &&
    !exists( $$DESTDIR\QtSolutions_SOAP-2.7.dll ):QMAKE_PRE_LINK += copy \
    $$(MAF3_FOUNDATION_LIB_DIR)\qtsoap\lib\*.dll \
    $$DESTDIR \
    &&
    !exists( $$DESTDIR\qscxml.dll ):QMAKE_PRE_LINK += copy \
    $$(MAF3_FOUNDATION_LIB_DIR)\qscxml\lib\*.dll \
    $$DESTDIR
}


unix {
    !exists( $$DESTDIR/*qxmlrpc.* ):QMAKE_PRE_LINK += cd $$(MAF3_FOUNDATION_LIB_DIR)/qxmlrpc/lib;tar -cf zipped *;mv zipped $$DESTDIR/;cd $$DESTDIR/;tar -xvf zipped;rm zipped;
    !exists( $$DESTDIR/*QtSolutions_SOAP.* ):QMAKE_PRE_LINK += cd $$(MAF3_FOUNDATION_LIB_DIR)/qtsoap/lib;tar -cf zipped *;mv zipped $$DESTDIR/;cd $$DESTDIR/;tar -xvf zipped;rm zipped;
    !exists( $$DESTDIR/*qscxml.* ):QMAKE_PRE_LINK += cd $$(MAF3_FOUNDATION_LIB_DIR)/qscxml/lib;tar -cf zipped *;mv zipped $$DESTDIR/;cd $$DESTDIR/;tar -xvf zipped;rm zipped;
}


