import qbs.base 1.0

Project{
	name: "ACF-Solutions"

	Project{
		name: "ACF-Solutions-Static"

		references: [
			"../../Include/iauth/Qbs/iauth.qbs",
			"../../Include/ihotf/Qbs/ihotf.qbs",
			"../../Include/ihotfgui/Qbs/ihotfgui.qbs",
			"../../Include/imeas/Qbs/imeas.qbs",
			"../../Include/iqtmeas/Qbs/iqtmeas.qbs",
			"../../Include/iqtex/Qbs/iqtex.qbs",
			"../../Include/imm/Qbs/imm.qbs",
			"../../Include/iqtmm/Qbs/iqtmm.qbs",
			"../../Include/iprop/Qbs/iprop.qbs",
			"../../Include/icam/Qbs/icam.qbs",
			"../../Include/iqtauth/Qbs/iqtauth.qbs",
			"../../Include/iqtcam/Qbs/iqtcam.qbs",
			"../../Include/iinsp/Qbs/iinsp.qbs",
			"../../Include/iqtinsp/Qbs/iqtinsp.qbs",
			"../../Include/iipr/Qbs/iipr.qbs",
			"../../Include/iqtipr/Qbs/iqtipr.qbs",
			"../../Include/isig/Qbs/isig.qbs",
			"../../Include/iqtsig/Qbs/iqtsig.qbs",
			"../../Include/ifileproc/Qbs/ifileproc.qbs",
			"../../Include/iproc/Qbs/iproc.qbs",
			"../../Include/iprocgui/Qbs/iprocgui.qbs",
			"../../Include/iwiz/Qbs/iwiz.qbs",
			"../../Include/iwizgui/Qbs/iwizgui.qbs",
			"../../Include/iedge/Qbs/iedge.qbs",
			"../../Include/iedgegui/Qbs/iedgegui.qbs",
			"../../Include/icmpstr/Qbs/icmpstr.qbs",
			"../../Include/idocproc/Qbs/idocproc.qbs",
			"../../Impl/AcfSlnLoc/Qbs/AcfSlnLoc.qbs"
		]
	}

	Project{
		name: "ACF-Solutions-Component"

		references: [
			"../../Impl/AuthorizationPck/Qbs/AuthorizationPck.qbs",
			"../../Impl/ProcessingPck/Qbs/ProcessingPck.qbs",
			"../../Impl/FileProcessingPck/Qbs/FileProcessingPck.qbs",
			"../../Impl/CameraPck/Qbs/CameraPck.qbs",
			"../../Impl/EdgePck/Qbs/EdgePck.qbs",
			"../../Impl/HotfolderPck/Qbs/HotfolderPck.qbs",
			"../../Impl/InspectionPck/Qbs/InspectionPck.qbs",
			"../../Impl/IprPck/Qbs/IprPck.qbs",
			"../../Impl/MeasPck/Qbs/MeasPck.qbs",
			"../../Impl/PropPck/Qbs/PropPck.qbs",
			"../../Impl/WizardPck/Qbs/WizardPck.qbs",
			"../../Impl/QtAuthorizationPck/Qbs/QtAuthorizationPck.qbs",
			"../../Impl/QtCameraPck/Qbs/QtCameraPck.qbs",
			"../../Impl/QtExPck/Qbs/QtExPck.qbs",
			"../../Impl/QtInspectionPck/Qbs/QtInspectionPck.qbs",
			"../../Impl/QtIprPck/Qbs/QtIprPck.qbs",
			"../../Impl/QtMeasPck/Qbs/QtMeasPck.qbs",
			"../../Impl/QtMmPck/Qbs/QtMmPck.qbs",
			"../../Impl/QtSignalPck/Qbs/QtSignalPck.qbs",
			"../../Impl/WizardGuiPck/Qbs/WizardGuiPck.qbs",
			"../../Impl/HotfolderGuiPck/Qbs/HotfolderGuiPck.qbs",
			"../../Impl/ProcessingGuiPck/Qbs/ProcessingGuiPck.qbs",
			"../../Impl/CompositorPck/Qbs/CompositorPck.qbs",
			"../../Impl/DocumentProcessingPck/Qbs/DocumentProcessingPck.qbs"
		]
	}

	Project{
		name: "ACF-Solutions-Tool"

		references: [
//			"../../Impl/AcfStarterExe/Qbs/AcfStarter.qbs",
			"../../Impl/CompositorExe/Qbs/Compositor.qbs",
			"../../Impl/XpcEditorExe/Qbs/XpcEditor.qbs"
		]
	}

	Product{
		name: "AcfSlnConfig"
		type: ["xpc", "acf_share"]

		files: ["../../Config/PartituraOnly.xpc"]

		destinationDirectory: "Config"

		Depends{ name: "acf" }
		Depends{ name: "AuthorizationPck" }
		Depends{ name: "ProcessingPck" }
		Depends{ name: "FileProcessingPck" }
		Depends{ name: "CameraPck" }
		Depends{ name: "EdgePck" }
		Depends{ name: "HotfolderPck" }
		Depends{ name: "InspectionPck" }
		Depends{ name: "IprPck" }
		Depends{ name: "MeasPck" }
		Depends{ name: "PropPck" }
		Depends{ name: "WizardPck" }
		Depends{ name: "QtAuthorizationPck" }
		Depends{ name: "QtCameraPck" }
		Depends{ name: "QtExPck" }
		Depends{ name: "QtInspectionPck" }
		Depends{ name: "QtIprPck" }
		Depends{ name: "QtMeasPck" }
		Depends{ name: "QtMmPck" }
		Depends{ name: "QtSignalPck" }
		Depends{ name: "WizardGuiPck" }
		Depends{ name: "HotfolderGuiPck" }
		Depends{ name: "ProcessingGuiPck" }
		Depends{ name: "CompositorPck" }
		Depends{ name: "DocumentProcessingPck" }
		Depends{ name: "AcfCoreConfig" }
	}
}
