import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "FileProcessingPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ifileproc" }

	Export{
		Depends{ name: "ifileproc" }
	}
}
