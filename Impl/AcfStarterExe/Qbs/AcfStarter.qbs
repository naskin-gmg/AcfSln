import qbs.base 1.0
import "../../../Config/Qbs/ApplicationProduct.qbs" as ApplicationProduct

ApplicationProduct{
	name: "AcfStarter"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "ipackage" }
	Depends{ name: "icomp" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "Qt.gui" }
}
