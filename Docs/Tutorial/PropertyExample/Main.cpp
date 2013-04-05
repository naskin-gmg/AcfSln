#include "CPropertyObjectExampe.h"

#include "ifile/CXmlFileWriteArchive.h"
#include "ifile/CXmlFileReadArchive.h"


int main(int /*argc*/, char* /*argv*/[])
{
	CPropertyObjectExampe myObject;

	{
		// write unchanged object to the file:
		ifile::CXmlFileWriteArchive archive("MyFile.xml");
		myObject.Serialize(archive);
	}

	// change properties:
	myObject.SetNumber(42);
	myObject.SetText("bla bla bla");
	myObject.SetRectangle(i2d::CRectangle(42, 42, 1000, 1000));

	// write changed object to the file:
	ifile::CXmlFileWriteArchive archive2("MyModifiedFile.xml");
	myObject.Serialize(archive2);

	// read original data from file:
	ifile::CXmlFileReadArchive archive3("MyFile.xml");
	myObject.Serialize(archive3);

	// and write restored object to the file:
	ifile::CXmlFileWriteArchive archive4("RestoredFile.xml");
	myObject.Serialize(archive4);

	return 0;
}
