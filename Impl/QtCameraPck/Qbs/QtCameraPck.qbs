import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "QtCameraPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtcam" }

	Export{
		Depends{ name: "iqtcam" }
	}
}
