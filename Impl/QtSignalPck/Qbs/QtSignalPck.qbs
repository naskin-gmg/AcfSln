import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "QtSignalPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtsig" }

	Export{
		Depends{ name: "iqtsig" }
	}
}
