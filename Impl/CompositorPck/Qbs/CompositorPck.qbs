import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "CompositorPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "icmpstr" }

	Export{
		Depends{ name: "icmpstr" }
	}
}
