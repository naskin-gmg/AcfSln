import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "edgetest"

    files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iedge" }
	Depends{ name: "iedgegui" }
    Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "iedge" }
		Depends{ name: "iedgegui" }
        Depends{ name: "Qt.widgets" }
    }
}
