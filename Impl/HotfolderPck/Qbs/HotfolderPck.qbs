import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "HotfolderPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ihotf" }

	Export{
		Depends{ name: "ihotf" }
	}
}
