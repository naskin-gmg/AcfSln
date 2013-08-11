import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "MeasPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "imeas" }

	Export{
		Depends{ name: "imeas" }
	}
}
