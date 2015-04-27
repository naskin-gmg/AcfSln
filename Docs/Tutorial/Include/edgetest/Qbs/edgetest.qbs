import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "edgetest"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iedge" }
	Depends{ name: "iedgegui" }

	Export{
		Depends{ name: "iedge" }
		Depends{ name: "iedgegui" }
	}
}
