import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "QtInspectionPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtinsp" }

	Export{
		Depends{ name: "iqtinsp" }
	}
}
