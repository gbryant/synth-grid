#-------------------------------------------------
#
# Project created by QtCreator 2011-09-20T15:32:14
#
#-------------------------------------------------

QT       += core gui phonon xml

CONFIG +=link_pkgconfig

TARGET = SynthGrid
TEMPLATE = app




#INCLUDEPATH += /opt/local/include/gstreamer-0.10 \
#               /opt/local/include/glib-2.0 \
#               /opt/local/lib/glib-2.0/include \
#               /opt/local/include/libxml2 \
#               /opt/local/include


INCLUDEPATH += "C:\Program Files (x86)\Mega-Nerd\libsndfile\include"

#PKG_CONFIG_PATH=/opt/local/lib/pkgconfig
#PKG_CONFIG=/opt/local/bin/pkg-config

#PKGCONFIG += gstreamer-0.10 \
#             sndfile
#             libpulse-simple \
#



#LIBS += /opt/local/lib/libsndfile.dylib \
#        /opt/local/lib/libglib-2.0.dylib \
#        /opt/local/lib/libgstreamer-0.10.dylib

LIBS += "C:\Program Files (x86)\Mega-Nerd\libsndfile\lib\libsndfile-1.lib"

INCLUDEPATH += src/synth/ \
               src/score_file/ \
               src/main/ \
               src/file_parsers/

FORMS    += \
    mainwindow.ui \
    synthparameters.ui \
    scoreparameters.ui

HEADERS += \
    src/synth/synthparameters.h \
    src/synth/oscillator.h \
    src/synth/instrument.h \
    src/score_file/scoreview.h \
    src/score_file/scoreparameters.h \
    src/score_file/scoredocument.h \
    src/score_file/scoredata.h \
    src/score_file/notation.h \
    src/main/mainwindow.h \
    src/file_parsers/scoredocumentparser.h \
    src/file_parsers/instrumentparser.h \
    src/synth/synthesizer.h \
    src/main/mymdisubwindow.h

SOURCES += \
    src/synth/synthparameters.cpp \
    src/synth/oscillator.cpp \
    src/synth/instrument.cpp \
    src/score_file/scoreview.cpp \
    src/score_file/scoreparameters.cpp \
    src/score_file/scoredocument.cpp \
    src/score_file/scoredata.cpp \
    src/main/mainwindow.cpp \
    src/main/main.cpp \
    src/file_parsers/scoredocumentparser.cpp \
    src/file_parsers/instrumentparser.cpp \
    src/synth/synthesizer.cpp \
    src/main/mymdisubwindow.cpp






















































