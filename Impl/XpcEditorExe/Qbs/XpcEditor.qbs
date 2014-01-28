import qbs.base 1.0
import AcfApplication as AcfApplication
import AcfStaticLibrary as AcfStaticLibrary

Project{
	StaticLibrary{
		name: "_XpcEditor"

		destinationDirectory: "GeneratedFiles/XpcEditor"

		files: ["../*.arx", "../*.qrc"]

		Depends{ name: "acf" }

        Depends{ name: "ArxcExe" }

		Depends{ name: "AcfSlnConfig" }
		Depends{ name: "CompositorPck" }
		Depends{ name: "QtGuiPck" }
		Depends{ name: "QtPck" }
		Depends{ name: "FilePck" }
		Depends{ name: "BasePck" }
		Depends{ name: "PackagePck" }

		acf.generatedOutputDir: "GeneratedFiles/XpcEditor"
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

		Group{
			name: "VC-Resources"
			condition: qbs.toolchain.contains("msvc")

			files: ["../VC/*.rc.xtracf"]
			fileTags: ["xtracf"]

			acf.trRegFile: "../../../Partitura/AcfInfoCopyApp.arx"
			acf.trOutputType: ["rc"]
		}
	}
}
