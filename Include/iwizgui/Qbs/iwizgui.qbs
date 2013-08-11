import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iwizgui"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iwiz" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "iwiz" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.widgets" }
	}
}
