import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "iservice"

    files: [
        "../C*.h", "../C*.cpp",
        "../qtservice.h", "../qtservice.cpp",
        "../qtservicestarter.h", "../qtservicestarter.cpp",
        "../qt*_win.h", "../qt*_win.cpp"]

	Depends{ name: "ibase" }
    Depends{ name: "Qt.gui" }
    Depends{ name: "Qt.network" }

	Export{
		Depends{ name: "ibase" }
        Depends{ name: "Qt.gui" }
        Depends{ name: "Qt.network" }
	}
}
