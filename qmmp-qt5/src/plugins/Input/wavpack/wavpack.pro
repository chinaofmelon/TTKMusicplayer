include($$PWD/../../plugins.pri)

HEADERS += decoderwavpackfactory.h \
           decoder_wavpack.h \
           cueparser.h \
           wavpackmetadatamodel.h
    
SOURCES += decoderwavpackfactory.cpp \
           decoder_wavpack.cpp \
           cueparser.cpp \
           wavpackmetadatamodel.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = wavpack

INCLUDEPATH += $$EXTRA_PREFIX/libwavpack/include

unix {
    unix:android {
        TARGET = plugin_input_wavpack
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libplugin_input_wavpack.so
    }else{
        QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libwavpack.so
    }
    LIBS += -L$$EXTRA_PREFIX/libwavpack/lib -lwavpack$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libwavpack/lib -lwavpack
}