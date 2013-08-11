import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "PropPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iprop" }

	Export{
		Depends{ name: "iprop" }
	}
}
