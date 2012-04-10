#ifndef CPropertyObjectExampe_included
#define CPropertyObjectExampe_included


// ACF includes
#include "iprop/CPropertiesManager.h"
#include "iprop/TObjectProperty.h"


// Project includes
#include "IMyDataModel.h"


/**
	Demostration, how to create an implementation of a persistent data object over property manager.
*/
class CPropertyObjectExampe:
			virtual public IMyDataModel,
			public iprop::CPropertiesManager
{
public:
	typedef iprop::CPropertiesManager BaseClass;

	CPropertyObjectExampe();

	// reimplemented (IMyDataModel)
	virtual double GetNumber() const;
	virtual void SetNumber(double number);
	virtual QByteArray GetText() const;
	virtual void SetText(const QByteArray& text);
	virtual i2d::CRectangle GetRectangle() const;
	virtual void SetRectangle(const i2d::CRectangle& rectangle);

private:
	// specify the data model over properties:
	iprop::CDoubleProperty m_number;
	iprop::CStdStringProperty m_text;

	// Rectangle property is no standard property, so we have to define our own property type:
	typedef iprop::TObjectProperty<i2d::CRectangle> CRectangleProperty;
	
	CRectangleProperty m_rectangle;
};


#endif // !CPropertyObjectExampe_included


