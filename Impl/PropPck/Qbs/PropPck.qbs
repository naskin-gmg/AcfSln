import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "PropPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iprop" }

	Export{
		Depends{ name: "iprop" }
	}
}
