include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Impl/AcfStd/QMake/AcfStd.pri)

TARGET = FfMpegPck

QT -= gui core

INCLUDEPATH += ../../../Include
INCLUDEPATH += $(FFMPEGDIR)/include $(FFMPEGDIR)/msinttypes

LIBS += -L../../../Lib/$$COMPILER_DIR -lilibav
LIBS += -L$(FFMPEGDIR)/lib -lswscale-0 -lavdevice-52 -lavformat-52 -lavcodec-52 -lavutil-49