import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqtauth"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iauth" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "iauth" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.widgets" }
	}
}
