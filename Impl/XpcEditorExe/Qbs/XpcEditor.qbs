import qbs.base 1.0
import "../../../Config/Qbs/ApplicationProduct.qbs" as ApplicationProduct

ApplicationProduct{
	name: "XpcEditor"

	files: ["../*.cpp", "../*.qrc", "../*.arx"]

	acf.acfConfigurationFile: "../../../Config/Full.xpc"

	Depends{ name: "Arxc" }

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
