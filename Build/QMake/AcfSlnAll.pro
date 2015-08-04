# Root of ACF-Solutions project

TEMPLATE = subdirs
CONFIG += ordered


# Static libraries

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

SUBDIRS += iblob
iblob.file = ../../Include/iblob/QMake/iblob.pro

SUBDIRS += iblobgui
iblobgui.file = ../../Include/iblobgui/QMake/iblobgui.pro

SUBDIRS += iservice
iservice.file = ../../Include/iservice/QMake/iservice.pro


# Translations

SUBDIRS += AcfSlnLoc
AcfSlnLoc.file = ../../Impl/AcfSlnLoc/QMake/AcfSlnLoc.pro


# Component packages

SUBDIRS += AuthorizationPck
AuthorizationPck.file = ../../Impl/AuthorizationPck/QMake/AuthorizationPck.pro
AuthorizationPck.depends = iauth

SUBDIRS += BlobPck
BlobPck.file = ../../Impl/BlobPck/QMake/BlobPck.pro
BlobPck.depends = iblob

SUBDIRS += ProcessingPck
ProcessingPck.file = ../../Impl/ProcessingPck/QMake/ProcessingPck.pro
ProcessingPck.depends = iproc

SUBDIRS += FileProcessingPck
FileProcessingPck.file = ../../Impl/FileProcessingPck/QMake/FileProcessingPck.pro
FileProcessingPck.depends = iproc ifileproc

SUBDIRS += CameraPck
CameraPck.file = ../../Impl/CameraPck/QMake/CameraPck.pro
CameraPck.depends = icam iproc

SUBDIRS += EdgePck
EdgePck.file = ../../Impl/EdgePck/QMake/EdgePck.pro
EdgePck.depends = iedge iproc

SUBDIRS += HotfolderPck
HotfolderPck.file = ../../Impl/HotfolderPck/QMake/HotfolderPck.pro
HotfolderPck.depends = ihotf ifileproc

SUBDIRS += InspectionPck
InspectionPck.file = ../../Impl/InspectionPck/QMake/InspectionPck.pro
InspectionPck.depends = iinsp iproc

SUBDIRS += IprPck
IprPck.file = ../../Impl/IprPck/QMake/IprPck.pro
IprPck.depends = iipr iproc imeas

SUBDIRS += MeasPck
MeasPck.file = ../../Impl/MeasPck/QMake/MeasPck.pro
MeasPck.depends = iproc

SUBDIRS += SignalPck
SignalPck.file = ../../Impl/SignalPck/QMake/SignalPck.pro
SignalPck.depends = isig

SUBDIRS += WizardPck
WizardPck.file = ../../Impl/WizardPck/QMake/WizardPck.pro
WizardPck.depends = iwiz iproc

SUBDIRS += QtAuthorizationPck
QtAuthorizationPck.file = ../../Impl/QtAuthorizationPck/QMake/QtAuthorizationPck.pro
QtAuthorizationPck.depends = iauth iqtauth

SUBDIRS += QtCameraPck
QtCameraPck.file = ../../Impl/QtCameraPck/QMake/QtCameraPck.pro
QtCameraPck.depends = icam iqtcam

SUBDIRS += QtExPck
QtExPck.file = ../../Impl/QtExPck/QMake/QtExPck.pro
QtExPck.depends = iqtext

SUBDIRS += QtInspectionPck
QtInspectionPck.file = ../../Impl/QtInspectionPck/QMake/QtInspectionPck.pro
QtInspectionPck.depends = iinsp iqtinsp

SUBDIRS += QtIprPck
QtIprPck.file = ../../Impl/QtIprPck/QMake/QtIprPck.pro
QtIprPck.depends = iipr iqtipr

SUBDIRS += QtMeasPck
QtMeasPck.file = ../../Impl/QtMeasPck/QMake/QtMeasPck.pro
QtMeasPck.depends = imeas iqtmeas

SUBDIRS += QtMmPck
QtMmPck.file = ../../Impl/QtMmPck/QMake/QtMmPck.pro
QtMmPck.depends = imm

SUBDIRS += QtSignalPck
QtSignalPck.file = ../../Impl/QtSignalPck/QMake/QtSignalPck.pro
QtSignalPck.depends = isig iqtsig

SUBDIRS += WizardGuiPck
WizardGuiPck.file = ../../Impl/WizardGuiPck/QMake/WizardGuiPck.pro
WizardGuiPck.depends = iwiz iwizgui

SUBDIRS += HotfolderGuiPck
HotfolderGuiPck.file = ../../Impl/HotfolderGuiPck/QMake/HotfolderGuiPck.pro
HotfolderGuiPck.depends = ihotf ihotfgui

SUBDIRS += ProcessingGuiPck
ProcessingGuiPck.file = ../../Impl/ProcessingGuiPck/QMake/ProcessingGuiPck.pro
ProcessingGuiPck.depends = iproc iprocgui

SUBDIRS += CompositorPck
CompositorPck.file = ../../Impl/CompositorPck/QMake/CompositorPck.pro
CompositorPck.depends = icmpstr

SUBDIRS += DocumentProcessingPck
DocumentProcessingPck.file = ../../Impl/DocumentProcessingPck/QMake/DocumentProcessingPck.pro
DocumentProcessingPck.depends = iproc idocproc


# Tools
SUBDIRS += Compositor
Compositor.file = ../../Impl/CompositorExe/QMake/Compositor.pro
Compositor.depends = CompositorPck

SUBDIRS += XpcEditor
XpcEditor.file = ../../Impl/XpcEditorExe/QMake/XpcEditor.pro
XpcEditor.depends = CompositorPck

