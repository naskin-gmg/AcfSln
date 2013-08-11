import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "CameraPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "icam" }

	Export{
		Depends{ name: "icam" }
	}
}
