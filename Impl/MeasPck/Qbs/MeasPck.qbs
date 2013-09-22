import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "MeasPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "imeas" }

	Export{
		Depends{ name: "imeas" }
	}
}
