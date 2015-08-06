import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "iservice"

    files: [
        "../C*.h", "../C*.cpp",
        "../qtservice.h", "../qtservice.cpp",
        "../qtservicestarter.h", "../qtservicestarter.cpp"]

    Group{
        name: "Windows-Resources"
        condition: qbs.targetOS.contains("windows")

        files: ["../qt*_win.h", "../qt*_win.cpp"]
    }

    Group{
        name: "Unix"
        condition: qbs.targetOS.contains("unix") || qbs.targetOS.contains("linux") || qbs.targetOS.contains("osx")

        files: [
            "../qt*_unix.h", "../qt*_unix.cpp",
            "../qtunix*.h", "../qtunix*.cpp"]
    }

    Depends{ name: "ibase" }
    Depends{ name: "Qt.gui" }
    Depends{ name: "Qt.network" }

	Export{
		Depends{ name: "ibase" }
        Depends{ name: "Qt.gui" }
        Depends{ name: "Qt.network" }
	}
}
