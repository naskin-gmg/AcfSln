import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "QtCameraPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtcam" }

	Export{
		Depends{ name: "iqtcam" }
	}
}
