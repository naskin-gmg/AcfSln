import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqtsig"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "isig" }
	Depends{ name: "imeas" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "Qt.script" }

	Export{
		Depends{ name: "isig" }
		Depends{ name: "imeas" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.widgets" }
		Depends{ name: "Qt.script" }
	}
}
