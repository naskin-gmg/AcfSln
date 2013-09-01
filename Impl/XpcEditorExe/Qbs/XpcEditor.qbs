import qbs.base 1.0
import "../../../Config/Qbs/ApplicationProduct.qbs" as ApplicationProduct
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

Project{
	StaticLibrary{
		name: "_XpcEditor"

		destinationDirectory: "GeneratedFiles/XpcEditor"

		files: ["../*.arx", "../*.qrc"]

		acf.generatedOutputDir: "GeneratedFiles/XpcEditor"

		Depends{ name: "Arxc" }

		Depends{ name: "AcfSlnConfig" }
		Depends{ name: "CompositorPck" }
		Depends{ name: "QtGuiPck" }
		Depends{ name: "QtPck" }
		Depends{ name: "FilePck" }
		Depends{ name: "BasePck" }
		Depends{ name: "PackagePck" }
		Depends{ name: "acf" }
	}

	ApplicationProduct{
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
