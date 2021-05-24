include($$PWD/../../plugins.pri)
include($$PWD/../../Visual/common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Light
TARGET = $${TARGET}

SOURCES += \
        spek-ruler.cc \
        spek-spectrogram.cc \
        spek-audio.cc \
        spek-fft.cc \
        spek-pipeline.cc \
        lightspectrumfactory.cpp

HEADERS  += \
        spek-audio.h \
        spek-fft.h \
        spek-pipeline.h \
        spek-ruler.h \
        spek-spectrogram.h \
        lightspectrumfactory.h

INCLUDEPATH += $$EXTRA_PREFIX/libffmpeg/include

win32:{
    LIBS += -L$$EXTRA_PREFIX/libffmpeg/lib -lavcodec.dll -lavformat.dll -lavutil.dll
}

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libffmpeg/lib -lavcodec -lavformat -lavutil
}