# Root of ACF-Solutions project

include ($(ACFCONFIGDIR)/QMake/GeneralConfig.pri)

TEMPLATE = subdirs
CONFIG += ordered


#Static

SUBDIRS += iauth
iauth.file = ../../Include/iauth/QMake/iauth.pro

SUBDIRS += ihotf
ihotf.file = ../../Include/ihotf/QMake/ihotf.pro

SUBDIRS += ihotfgui
ihotfgui.file = ../../Include/ihotfgui/QMake/ihotfgui.pro

SUBDIRS += imeas
imeas.file = ../../Include/imeas/QMake/imeas.pro

SUBDIRS += iqtmeas
iqtmeas.file = ../../Include/iqtmeas/QMake/iqtmeas.pro

SUBDIRS += iqtex
iqtex.file = ../../Include/iqtex/QMake/iqtex.pro

SUBDIRS += iqtmm
iqtmm.file = ../../Include/iqtmm/QMake/iqtmm.pro

SUBDIRS += iprop
iprop.file = ../../Include/iprop/QMake/iprop.pro

SUBDIRS += icam
icam.file = ../../Include/icam/QMake/icam.pro

SUBDIRS += iqtauth
iqtauth.file = ../../Include/iqtauth/QMake/iqtauth.pro

SUBDIRS += iqtcam
iqtcam.file = ../../Include/iqtcam/QMake/iqtcam.pro

SUBDIRS += iinsp
iinsp.file = ../../Include/iinsp/QMake/iinsp.pro

SUBDIRS += iqtinsp
iqtinsp.file = ../../Include/iqtinsp/QMake/iqtinsp.pro

SUBDIRS += iipr
iipr.file = ../../Include/iipr/QMake/iipr.pro

SUBDIRS += iqtipr
iqtipr.file = ../../Include/iqtipr/QMake/iqtipr.pro

SUBDIRS += isig
isig.file = ../../Include/isig/QMake/isig.pro

SUBDIRS += iqtsig
iqtsig.file = ../../Include/iqtsig/QMake/iqtsig.pro

SUBDIRS += ifileproc
ifileproc.file = ../../Include/ifileproc/QMake/ifileproc.pro

SUBDIRS += iproc
iproc.file = ../../Include/iproc/QMake/iproc.pro

SUBDIRS += iprocgui
iprocgui.file = ../../Include/iprocgui/QMake/iprocgui.pro

SUBDIRS += iwiz
iwiz.file = ../../Include/iwiz/QMake/iwiz.pro

SUBDIRS += iwizgui
iwizgui.file = ../../Include/iwizgui/QMake/iwizgui.pro

SUBDIRS += iedge
iedge.file = ../../Include/iedge/QMake/iedge.pro

SUBDIRS += iedgegui
iedgegui.file = ../../Include/iedgegui/QMake/iedgegui.pro

SUBDIRS += icmpstr
icmpstr.file = ../../Include/icmpstr/QMake/icmpstr.pro

SUBDIRS += idocproc
idocproc.file = ../../Include/idocproc/QMake/idocproc.pro


# Translation

SUBDIRS += AcfSlnLoc
AcfSlnLoc.file = ../../Impl/AcfSlnLoc/QMake/AcfSlnLoc.pro


#Component

SUBDIRS += AuthorizationPck
AuthorizationPck.file = ../../Impl/AuthorizationPck/QMake/AuthorizationPck.pro

SUBDIRS += ProcessingPck
ProcessingPck.file = ../../Impl/ProcessingPck/QMake/ProcessingPck.pro

SUBDIRS += FileProcessingPck
FileProcessingPck.file = ../../Impl/FileProcessingPck/QMake/FileProcessingPck.pro

SUBDIRS += CameraPck
CameraPck.file = ../../Impl/CameraPck/QMake/CameraPck.pro

SUBDIRS += EdgePck
EdgePck.file = ../../Impl/EdgePck/QMake/EdgePck.pro

SUBDIRS += HotfolderPck
HotfolderPck.file = ../../Impl/HotfolderPck/QMake/HotfolderPck.pro

SUBDIRS += InspectionPck
InspectionPck.file = ../../Impl/InspectionPck/QMake/InspectionPck.pro

SUBDIRS += IprPck
IprPck.file = ../../Impl/IprPck/QMake/IprPck.pro

SUBDIRS += MeasPck
MeasPck.file = ../../Impl/MeasPck/QMake/MeasPck.pro

SUBDIRS += PropPck
PropPck.file = ../../Impl/PropPck/QMake/PropPck.pro

SUBDIRS += WizardPck
WizardPck.file = ../../Impl/WizardPck/QMake/WizardPck.pro

SUBDIRS += QtAuthorizationPck
QtAuthorizationPck.file = ../../Impl/QtAuthorizationPck/QMake/QtAuthorizationPck.pro

SUBDIRS += QtCameraPck
QtCameraPck.file = ../../Impl/QtCameraPck/QMake/QtCameraPck.pro

SUBDIRS += QtExPck
QtExPck.file = ../../Impl/QtExPck/QMake/QtExPck.pro

SUBDIRS += QtInspectionPck
QtInspectionPck.file = ../../Impl/QtInspectionPck/QMake/QtInspectionPck.pro

SUBDIRS += QtIprPck
QtIprPck.file = ../../Impl/QtIprPck/QMake/QtIprPck.pro

SUBDIRS += QtMeasPck
QtMeasPck.file = ../../Impl/QtMeasPck/QMake/QtMeasPck.pro

SUBDIRS += QtMmPck
QtMmPck.file = ../../Impl/QtMmPck/QMake/QtMmPck.pro

SUBDIRS += QtSignalPck
QtSignalPck.file = ../../Impl/QtSignalPck/QMake/QtSignalPck.pro

SUBDIRS += WizardGuiPck
WizardGuiPck.file = ../../Impl/WizardGuiPck/QMake/WizardGuiPck.pro

SUBDIRS += HotfolderGuiPck
HotfolderGuiPck.file = ../../Impl/HotfolderGuiPck/QMake/HotfolderGuiPck.pro

SUBDIRS += ProcessingGuiPck
ProcessingGuiPck.file = ../../Impl/ProcessingGuiPck/QMake/ProcessingGuiPck.pro

SUBDIRS += CompositorPck
CompositorPck.file = ../../Impl/CompositorPck/QMake/CompositorPck.pro

SUBDIRS += DocumentProcessingPck
DocumentProcessingPck.file = ../../Impl/DocumentProcessingPck/QMake/DocumentProcessingPck.pro


#Tool

SUBDIRS += Compositor
Compositor.file = ../../Impl/CompositorExe/QMake/Compositor.pro

SUBDIRS += XpcEditor
XpcEditor.file = ../../Impl/XpcEditorExe/QMake/XpcEditor.pro

SUBDIRS += AcfStarter
AcfStarter.file = ../../Impl/AcfStarterExe/QMake/AcfStarter.pro

