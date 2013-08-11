import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "QtExPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtex" }

	Export{
		Depends{ name: "iqtex" }
	}
}
