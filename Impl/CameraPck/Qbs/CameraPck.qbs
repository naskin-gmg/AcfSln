import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "CameraPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "icam" }

	Export{
		Depends{ name: "icam" }
	}
}
