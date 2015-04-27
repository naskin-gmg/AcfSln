import qbs.base 1.0

Project{
	name: "TutorialAcfSln"

	property string projectRoot: path + "/../../.."
	property string projectSufixDir: "Tutorial"

	Project{
		name: "TutorialAcfSln-Static"

		references: [
			"../../Include/edgetest/Qbs/edgetest.qbs"
		]
	}

	Project{
		name: "TutorialAcf-Component"

		references: [
			"../../Impl/TutorialAcfSln/Qbs/TutorialAcfSln.qbs"
		]
	}

	Product{
		name: "TutorialAcfSlnConfig"
		type: ["xpc", "acf_share"]

		files: ["../../../../Config/PartituraOnly.xpc"]

		destinationDirectory: "Config"

		acf.xpcPackageDirs: path + "/../../Partitura"

		Depends{ name: "acf" }
		Depends{ name: "TutorialAcfSln" }
		Depends{ name: "AcfSlnConfig" }
	}
}
