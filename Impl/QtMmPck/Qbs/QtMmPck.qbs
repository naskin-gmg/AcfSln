import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "QtMmPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtmm" }

	Export{
		Depends{ name: "iqtmm" }
	}
}
