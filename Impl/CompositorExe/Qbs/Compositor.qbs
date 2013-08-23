import qbs.base 1.0
import "../../../Config/Qbs/ApplicationProduct.qbs" as ApplicationProduct
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

Project{
	StaticLibrary{
		name: "_Compositor"

		destinationDirectory: "GeneratedFiles"

		files: ["../*.arx", "../*.qrc"]

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
		name: "Compositor"

		files: ["../*.cpp"]

		Depends{ name: "_Compositor" }

		Depends{ name: "AcfSlnLoc" }
		Depends{ name: "AcfLoc" }

		Depends{ name: "icmpstr" }
		Depends{ name: "iqtgui" }
		Depends{ name: "ifile" }
		Depends{ name: "ipackage" }

		Depends{ name: "Qt.widgets" }
		Depends{ name: "Qt.gui" }
	}
}
