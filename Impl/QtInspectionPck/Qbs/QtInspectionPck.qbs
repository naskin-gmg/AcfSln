import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "QtInspectionPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtinsp" }

	Export{
		Depends{ name: "iqtinsp" }
	}
}
