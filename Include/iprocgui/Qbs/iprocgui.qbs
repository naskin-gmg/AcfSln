import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iprocgui"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iproc" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "iproc" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.widgets" }
	}
}
