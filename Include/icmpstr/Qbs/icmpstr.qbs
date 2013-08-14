import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "icmpstr"

	files: ["../*.h", "../*.cpp", "../*.ui", "../*.qrc"]

	Depends{ name: "idoc" }
	Depends{ name: "iqtgui" }
	Depends{ name: "i2d" }
	Depends{ name: "ipackage" }
	Depends{ name: "Qt.gui" }
	Depends{ name: "Qt.printsupport" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "idoc" }
		Depends{ name: "iqtgui" }
		Depends{ name: "i2d" }
		Depends{ name: "ipackage" }
		Depends{ name: "Qt.gui" }
		Depends{ name: "Qt.printsupport" }
		Depends{ name: "Qt.widgets" }
	}
}
