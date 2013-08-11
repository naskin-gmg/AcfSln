import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqtex"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtgui" }
	Depends{ name: "iqt" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "Qt.gui" }
	Depends{ name: "Qt.network" }
	Depends{ name: "Qt.xmlpatterns" }

	Export{
		Depends{ name: "iqtgui" }
		Depends{ name: "iqt" }
		Depends{ name: "Qt.widgets" }
		Depends{ name: "Qt.gui" }
		Depends{ name: "Qt.network" }
		Depends{ name: "Qt.xmlpatterns" }
	}
}
