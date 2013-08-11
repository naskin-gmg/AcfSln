import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "QtAuthorizationPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtauth" }

	Export{
		Depends{ name: "iqtauth" }
	}
}
