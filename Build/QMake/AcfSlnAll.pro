#Root of ACF-Solutions project

include ($(ACFDIR)/Config/QMake/GeneralConfig.pri)

TEMPLATE = subdirs
CONFIG += ordered


#Static

SUBDIRS += iauth
SUBDIRS += ihotf
SUBDIRS += ihotfgui
SUBDIRS += imeas
SUBDIRS += iqtmeas
SUBDIRS += iqtex
SUBDIRS += iqtmm
SUBDIRS += iprop
SUBDIRS += icam
SUBDIRS += iqtauth
SUBDIRS += iqtcam
SUBDIRS += iinsp
SUBDIRS += iqtinsp
SUBDIRS += iipr
SUBDIRS += iqtipr
SUBDIRS += isig
SUBDIRS += iqtsig
SUBDIRS += ifileproc
SUBDIRS += iproc
SUBDIRS += iprocgui
SUBDIRS += iwiz
SUBDIRS += iwizgui
SUBDIRS += iedge
SUBDIRS += iedgegui

iauth.file = ../../Include/iauth/QMake/iauth.pro
ihotf.file = ../../Include/ihotf/QMake/ihotf.pro
ihotfgui.file = ../../Include/ihotfgui/QMake/ihotfgui.pro
imeas.file = ../../Include/imeas/QMake/imeas.pro
iqtmeas.file = ../../Include/iqtmeas/QMake/iqtmeas.pro
iqtex.file = ../../Include/iqtex/QMake/iqtex.pro
iqtmm.file = ../../Include/iqtmm/QMake/iqtmm.pro
iprop.file = ../../Include/iprop/QMake/iprop.pro
icam.file = ../../Include/icam/QMake/icam.pro
iqtauth.file = ../../Include/iqtauth/QMake/iqtauth.pro
iqtcam.file = ../../Include/iqtcam/QMake/iqtcam.pro
iinsp.file = ../../Include/iinsp/QMake/iinsp.pro
iqtinsp.file = ../../Include/iqtinsp/QMake/iqtinsp.pro
iipr.file = ../../Include/iipr/QMake/iipr.pro
iqtipr.file = ../../Include/iqtipr/QMake/iqtipr.pro
isig.file = ../../Include/isig/QMake/isig.pro
iqtsig.file = ../../Include/iqtsig/QMake/iqtsig.pro
ifileproc.file = ../../Include/ifileproc/QMake/ifileproc.pro
iproc.file = ../../Include/iproc/QMake/iproc.pro
iprocgui.file = ../../Include/iprocgui/QMake/iprocgui.pro
iwiz.file = ../../Include/iwiz/QMake/iwiz.pro
iwizgui.file = ../../Include/iwizgui/QMake/iwizgui.pro
iedge.file = ../../Include/iedge/QMake/iedge.pro
iedgegui.file = ../../Include/iedgegui/QMake/iedgegui.pro


#Component

SUBDIRS += ../../Impl/AuthorizationPck/QMake/AuthorizationPck.pro
SUBDIRS += ../../Impl/ProcessingPck/QMake/ProcessingPck.pro
SUBDIRS += ../../Impl/FileProcessingPck/QMake/FileProcessingPck.pro
SUBDIRS += ../../Impl/CameraPck/QMake/CameraPck.pro
SUBDIRS += ../../Impl/EdgePck/QMake/EdgePck.pro
SUBDIRS += ../../Impl/HotfolderPck/QMake/HotfolderPck.pro
SUBDIRS += ../../Impl/InspectionPck/QMake/InspectionPck.pro
SUBDIRS += ../../Impl/IprPck/QMake/IprPck.pro
SUBDIRS += ../../Impl/MeasPck/QMake/MeasPck.pro
SUBDIRS += ../../Impl/PropPck/QMake/PropPck.pro
SUBDIRS += ../../Impl/WizardPck/QMake/WizardPck.pro
SUBDIRS += ../../Impl/SignalPck/QMake/SignalPck.pro

SUBDIRS += ../../Impl/QtAuthorizationPck/QMake/QtAuthorizationPck.pro
SUBDIRS += ../../Impl/QtCameraPck/QMake/QtCameraPck.pro
SUBDIRS += ../../Impl/QtExPck/QMake/QtExPck.pro
SUBDIRS += ../../Impl/QtInspectionPck/QMake/QtInspectionPck.pro
SUBDIRS += ../../Impl/QtIprPck/QMake/QtIprPck.pro
SUBDIRS += ../../Impl/QtMeasPck/QMake/QtMeasPck.pro
SUBDIRS += ../../Impl/QtMmPck/QMake/QtMmPck.pro
SUBDIRS += ../../Impl/QtSignalPck/QMake/QtSignalPck.pro
SUBDIRS += ../../Impl/WizardGuiPck/QMake/WizardGuiPck.pro
SUBDIRS += ../../Impl/HotfolderGuiPck/QMake/HotfolderGuiPck.pro
SUBDIRS += ../../Impl/ProcessingGuiPck/QMake/ProcessingGuiPck.pro
