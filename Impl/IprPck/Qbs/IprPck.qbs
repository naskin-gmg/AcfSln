import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "IprPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iipr" }

	Export{
		Depends{ name: "iipr" }
	}
}
