import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtipr" }

	Export{
		Depends{ name: "iqtipr" }
	}
}
