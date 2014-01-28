import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "InspectionPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iinsp" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "iinsp" }
		Depends{ name: "icomp" }
	}
}
