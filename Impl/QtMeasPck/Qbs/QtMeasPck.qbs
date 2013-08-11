import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "QtMeasPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtmeas" }

	Export{
		Depends{ name: "iqtmeas" }
	}
}
