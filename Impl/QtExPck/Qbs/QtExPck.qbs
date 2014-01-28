import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "QtExPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtex" }

	Export{
		Depends{ name: "iqtex" }
	}
}
