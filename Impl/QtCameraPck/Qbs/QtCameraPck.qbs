import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "QtCameraPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtcam" }

	Export{
		Depends{ name: "iqtcam" }
	}
}
