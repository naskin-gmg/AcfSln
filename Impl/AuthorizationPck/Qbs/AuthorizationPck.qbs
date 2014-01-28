import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "AuthorizationPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iauth" }

	Export{
		Depends{ name: "iauth" }
	}
}
