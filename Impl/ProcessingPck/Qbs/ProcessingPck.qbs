import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "ProcessingPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iproc" }

	Export{
		Depends{ name: "iproc" }
	}
}
