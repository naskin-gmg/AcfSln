import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "HotfolderPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ihotf" }

	Export{
		Depends{ name: "ihotf" }
	}
}
