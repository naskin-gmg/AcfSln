#include "CPropertyObjectExampe.h"


// public methods

CPropertyObjectExampe::CPropertyObjectExampe()
	:m_number(this, /*Tag name*/"MyNumber", /*Tag description*/"A simple number", /* Property is persistent*/iprop::IProperty::PF_PERSISTENT),
	m_text(this, "MyText", "A small text", iprop::IProperty::PF_PERSISTENT),
	m_rectangle(this, "MyRectangle", "Rectangle object", iprop::IProperty::PF_PERSISTENT)
{
	// register your own property types:
	RegisterPropertyType<CRectangleProperty>();
}


// reimplemented (IMyDataModel)

double CPropertyObjectExampe::GetNumber() const
{
	return m_number.GetValue();
}


void CPropertyObjectExampe::SetNumber(double number)
{
	m_number.SetValue(number);
}


QByteArray CPropertyObjectExampe::GetText() const
{
	return m_text.GetValue();
}


void CPropertyObjectExampe::SetText(const QByteArray& text)
{
	m_text.SetValue(text);
}


i2d::CRectangle CPropertyObjectExampe::GetRectangle() const
{
	return m_rectangle.GetValue();
}


void CPropertyObjectExampe::SetRectangle(const i2d::CRectangle& rectangle)
{
	m_rectangle.SetValue(rectangle);
}

