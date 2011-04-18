include(../../../Config/QMake/Component.config)
include(../../AcfStd/QMake/AcfStd.pri)

TARGET = FfMpegPck

QT -= gui core

INCLUDEPATH += $(FFMPEGDIR)/include $(FFMPEGDIR)/msinttypes

LIBS += -lilibav
LIBS += -L$(FFMPEGDIR)/lib -lswscale-0 -lavdevice-52 -lavformat-52 -lavcodec-52 -lavutil-49