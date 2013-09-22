import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "QtIprPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtipr" }

	Export{
		Depends{ name: "iqtipr" }
	}
}
