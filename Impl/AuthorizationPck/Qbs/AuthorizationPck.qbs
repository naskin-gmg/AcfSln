import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "AuthorizationPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iauth" }

	Export{
		Depends{ name: "iauth" }
	}
}
