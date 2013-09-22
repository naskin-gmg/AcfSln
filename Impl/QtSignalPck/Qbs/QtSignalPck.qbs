import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "QtSignalPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtsig" }

	Export{
		Depends{ name: "iqtsig" }
	}
}
