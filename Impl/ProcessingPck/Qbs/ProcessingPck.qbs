import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "ProcessingPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iproc" }

	Export{
		Depends{ name: "iproc" }
	}
}
