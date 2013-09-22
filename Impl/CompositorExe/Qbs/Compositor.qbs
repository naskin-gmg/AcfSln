import qbs.base 1.0
import "../../../Config/Qbs/AcfApplication.qbs" as AcfApplication
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

Project{
	StaticLibrary{
		name: "_Compositor"

		destinationDirectory: "GeneratedFiles/Compositor"

		files: ["../*.arx", "../*.qrc"]

		acf.generatedOutputDir: "GeneratedFiles/Compositor"

		Depends{ name: "ArxcExe" }

		Depends{ name: "AcfSlnConfig" }

		Depends{ name: "CompositorPck" }
		Depends{ name: "QtGuiPck" }
		Depends{ name: "QtPck" }
		Depends{ name: "FilePck" }
		Depends{ name: "BasePck" }
		Depends{ name: "PackagePck" }

		Depends{ name: "acf" }
	}

	AcfApplication{
		name: "CompositorExe"
		targetName: "Compositor"

		consoleApplication: false

		files: ["../*.cpp"]

		Depends{ name: "_Compositor" }

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
