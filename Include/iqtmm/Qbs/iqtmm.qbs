import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqtmm"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "imm" }
	Depends{ name: "iimg" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "Qt.svg" }

	Export{
		Depends{ name: "imm" }
		Depends{ name: "iimg" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.widgets" }
		Depends{ name: "Qt.svg" }
	}
}
