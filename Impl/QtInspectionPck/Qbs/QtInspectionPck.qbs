import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "QtInspectionPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtinsp" }

	Export{
		Depends{ name: "iqtinsp" }
	}
}
