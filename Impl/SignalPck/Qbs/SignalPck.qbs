import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "SignalPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "isig" }

	Export{
		Depends{ name: "isig" }
	}
}
