#ifndef IMyDataModel_included
#define IMyDataModel_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include "iser/ISerializable.h"

#include "i2d/CRectangle.h"


/**
	A simple data model interface with persistence support.
*/
class IMyDataModel: virtual public iser::ISerializable
{
public:
	virtual double GetNumber() const = 0;
	virtual void SetNumber(double number) = 0;

	virtual QByteArray GetText() const = 0;
	virtual void SetText(const QByteArray& text) = 0;

	virtual i2d::CRectangle GetRectangle() const = 0;
	virtual void SetRectangle(const i2d::CRectangle& rectangle) = 0;
};


#endif // !IMyDataModel_included


