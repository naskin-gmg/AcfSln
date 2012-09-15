#include "iipr/CMultiLineSupplierCompBase.h"


// ACF includes
#include "iprm/TParamsPtr.h"


namespace iipr
{


// reimplemented (imeas::INumericValueProvider)

int CMultiLineSupplierCompBase::GetValuesCount() const
{
	const ProductType* resultPtr = GetWorkProduct();
	if (resultPtr != NULL){
		return resultPtr->count();
	}

	return 0;
}


const imeas::INumericValue& CMultiLineSupplierCompBase::GetNumericValue(int index) const
{
	static Line emptyLine;

	const ProductType* resultPtr = GetWorkProduct();
	if (resultPtr != NULL){
		const i2d::CLine2d& line = resultPtr->at(index);

		Line lineValue(line);
		m_linesCache[index] = lineValue;

		return m_linesCache[index];
	}

	return emptyLine;
}


CMultiLineSupplierCompBase::Line::Line()
{
}


CMultiLineSupplierCompBase::Line::Line(const i2d::CLine2d& line)
{
	m_values.SetElementsCount(4);

	m_values.SetElement(0, line.GetPoint1().GetX());
	m_values.SetElement(1, line.GetPoint1().GetY());
	m_values.SetElement(2, line.GetPoint2().GetX());
	m_values.SetElement(3, line.GetPoint2().GetY());
}


// reimplemented (imeas::INumericValue)

bool CMultiLineSupplierCompBase::Line::IsValueTypeSupported(ValueTypeId valueTypeId) const
{
	switch (valueTypeId){
		case VTI_AUTO:
		case VTI_2D_LINE:
			return true;
		default:
			return false;
	}
}


const imeas::INumericConstraints* CMultiLineSupplierCompBase::Line::GetNumericConstraints() const
{
	return NULL;
}


imath::CVarVector CMultiLineSupplierCompBase::Line::GetComponentValue(ValueTypeId valueTypeId) const
{
	switch (valueTypeId){
		case VTI_AUTO:
		case VTI_2D_LINE:
			return m_values;
		default:
			return imath::CVarVector();
	}
}


imath::CVarVector CMultiLineSupplierCompBase::Line::GetValues() const
{
	return m_values;
}


bool CMultiLineSupplierCompBase::Line::SetValues(const imath::CVarVector& values)
{
	m_values = values;

	return true;
}


// reimplemented (iser::ISerializable)

bool CMultiLineSupplierCompBase::Line::Serialize(iser::IArchive& /*archive*/)
{
	return false;
}


// reimplemented (istd::IChangeable)

bool CMultiLineSupplierCompBase::Line::CopyFrom(const IChangeable& object)
{
	const Line* linePtr = dynamic_cast<const Line*>(&object);
	if (linePtr != NULL){
		m_values = linePtr->m_values;

		return true;
	}

	return false;
}


} // namespace iipr


