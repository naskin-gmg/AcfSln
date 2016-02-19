import qbs.base 1.0
import AcfApplication as AcfApplication
import AcfLibrary as AcfLibrary

Project{
	StaticLibrary{
		name: "_XpcEditor"

		destinationDirectory: "GeneratedFiles/XpcEditor"

		files: ["../*.acc", "../*.qrc"]

		Depends{ name: "acf" }

		Depends{ name: "AcfSlnConfig" }
		Depends{ name: "CompositorPck" }
		Depends{ name: "QtGuiPck" }
		Depends{ name: "QtPck" }
        Depends{ name: "FilePck" }
        Depends{ name: "FileGuiPck" }
        Depends{ name: "BasePck" }
		Depends{ name: "PackagePck" }

		acf.generatedOutputDir: "GeneratedFiles/XpcEditor"

		Export{
			Depends{ name: "cpp" }
			cpp.includePaths: [product.buildDirectory]
		}
	}

	AcfApplication{
		name: "XpcEditorExe"
		targetName: "XpcEditor"

		consoleApplication: false

		files: ["../*.cpp"]

		Depends{ name: "_XpcEditor" }

		Depends{ name: "AcfSlnConfig" }
		Depends{ name: "AcfSlnLoc" }
		Depends{ name: "AcfLoc" }

		Depends{ name: "icmpstr" }
		Depends{ name: "iqtgui" }
		Depends{ name: "ifile" }
		Depends{ name: "ipackage" }

		Depends{ name: "Qt.widgets" }
		Depends{ name: "Qt.gui" }

		// Resource file on Windows
		acf.trRegFile: "../../../Partitura/AcfInfoCopyApp.acc"
		acf.trOutputType: "rc"
		cpp.includePaths: path

		Group{
			name: "Windows-Resources"
			condition: qbs.targetOS.contains("windows")

			files: ["../VC/*.rc.xtracf"]
			fileTags: ["xtracf"]
		}
	}
}
