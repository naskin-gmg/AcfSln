import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "QtIprPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtipr" }

	Export{
		Depends{ name: "iqtipr" }
	}
}
