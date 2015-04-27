import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "TutorialAcfSln"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "icomp" }
	Depends{ name: "edgetest" }

	Export{
		Depends{ name: "icomp" }
		Depends{ name: "edgetest" }
	}
}
