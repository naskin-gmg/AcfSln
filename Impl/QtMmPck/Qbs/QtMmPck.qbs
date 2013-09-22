import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "QtMmPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtmm" }

	Export{
		Depends{ name: "iqtmm" }
	}
}
