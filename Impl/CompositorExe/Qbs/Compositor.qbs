import qbs.base 1.0
import "../../../Config/Qbs/ApplicationProduct.qbs" as ApplicationProduct

Project{
	Product{
		name: "Compositor-Preconditions"
		type: "other"
		files: []

		Depends{ name: "CompositorPck" }
		Depends{ name: "QtGuiPck" }
		Depends{ name: "QtPck" }
		Depends{ name: "FilePck" }
		Depends{ name: "BasePck" }
		Depends{ name: "PackagePck" }
	}

	ApplicationProduct{
		name: "Compositor"

		acf.acfConfigurationFile: "../../../Config/Full.xpc"

		files: ["../*.cpp", "../*.qrc", "../*.arx"]

		Depends{ name: "Compositor-Preconditions" }
		Depends{ name: "AcfSlnLoc" }
		Depends{ name: "AcfLoc" }

		Depends{ name: "CompositorPck" }
		Depends{ name: "QtGuiPck" }
		Depends{ name: "QtPck" }
		Depends{ name: "FilePck" }
		Depends{ name: "BasePck" }
		Depends{ name: "PackagePck" }

		Depends{ name: "ibase" }
		Depends{ name: "ipackage" }

		Depends{ name: "acf" }
		Depends{ name: "Qt.widgets" }
		Depends{ name: "Qt.gui" }
	}
}
