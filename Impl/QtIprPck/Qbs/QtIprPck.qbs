import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "QtIprPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtipr" }

	Export{
		Depends{ name: "iqtipr" }
	}
}
