import qbs.base 1.0

Project{
	Depends{ name: "qbs" }

	property string acfDir: qbs.getenv("ACFDIR")

	qbsSearchPaths: [
		acfDir + "/Config/Qbs",
		acfDir + "/Config/Qbs/share"
	]

	SubProject{
		filePath: "./AcfSlnProject.qbs"
	}
}
