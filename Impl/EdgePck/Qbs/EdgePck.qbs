import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "EdgePck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iedgegui" }
	Depends{ name: "iedge" }

	Export{
		Depends{ name: "iedgegui" }
		Depends{ name: "iedge" }
	}
}
