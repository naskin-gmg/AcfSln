import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "FileProcessingPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ifileproc" }

	Export{
		Depends{ name: "ifileproc" }
	}
}
