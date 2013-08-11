import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iprop"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtgui" }
	Depends{ name: "iser" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "iqtgui" }
		Depends{ name: "iser" }
		Depends{ name: "Qt.widgets" }
	}
}
