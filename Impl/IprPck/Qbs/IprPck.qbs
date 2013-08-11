import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "IprPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iipr" }

	Export{
		Depends{ name: "iipr" }
	}
}
