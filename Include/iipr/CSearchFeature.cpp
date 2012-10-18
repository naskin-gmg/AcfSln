#include "iipr/CSearchFeature.h"


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


// public methods

CSearchFeature::CSearchFeature(
			double weight,
			const i2d::CVector2d& position,
			double angle,
			const i2d::CVector2d& scale,
			int index,
			const QString& id)
:	BaseClass(weight),
	m_scale(scale),
	m_angle(angle),
	m_index(index),
	m_id(id)
{
	BaseClass::SetPosition(position);
}


double CSearchFeature::GetAngle() const
{
	return m_angle;
}


const i2d::CVector2d& CSearchFeature::GetScale() const
{
	return m_scale;
}


int CSearchFeature::GetIndex() const
{
	return m_index;
}


const QString& CSearchFeature::GetId() const
{
	return m_id;
}


const i2d::ITransformation2d& CSearchFeature::GetTransformation() const
{
	m_transformation.Reset(GetPosition(), m_angle, m_scale);

	return m_transformation;
}


// reimplemented (imeas::INumericValue)

bool CSearchFeature::IsValueTypeSupported(CSearchFeature::ValueTypeId valueTypeId) const
{
	switch (valueTypeId){
		case VTI_AUTO:
		case VTI_POSITION:
		case VTI_ANGLE:
		case VTI_WEIGHT:
		case VTI_2D_TRANSFORM:
			return true;
	}

	return false;
}


imath::CVarVector CSearchFeature::GetComponentValue(CSearchFeature::ValueTypeId valueTypeId) const
{
	imath::CVarVector result;

	switch (valueTypeId){
		case VTI_AUTO:
		case VTI_POSITION:
			return BaseClass::GetComponentValue(VTI_AUTO);

		case VTI_ANGLE:
			result.SetElementsCount(1, m_angle);
			break;

		case VTI_WEIGHT:
			result.SetElementsCount(1, m_weight);
			break;

		case VTI_2D_TRANSFORM:
			result.SetElementsCount(6);
			{
				const i2d::CMatrix2d& matrix = m_transformation.GetTransformation().GetDeformMatrix();
				result.SetElement(0, matrix.GetAt(0,0));
				result.SetElement(1, matrix.GetAt(0,1));
				result.SetElement(2, matrix.GetAt(1,0));
				result.SetElement(3, matrix.GetAt(1,1));

				const i2d::CVector2d& translation = m_transformation.GetTransformation().GetTranslation();
				result.SetElement(4, translation.GetX());
				result.SetElement(5, translation.GetY());
			}
			break;
	}

	return result;
}


// reimplemented (iser::ISerializable)

bool CSearchFeature::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	retVal = retVal && BaseClass::Serialize(archive);

	static iser::CArchiveTag angleTag("Angle", "Angle of found model");
	retVal = retVal && archive.BeginTag(angleTag);
	retVal = retVal && archive.Process(m_angle);
	retVal = retVal && archive.EndTag(angleTag);

	static iser::CArchiveTag scaleTag("Scale", "Scale of found model");
	retVal = retVal && archive.BeginTag(scaleTag);
	retVal = retVal && m_scale.Serialize(archive);
	retVal = retVal && archive.EndTag(scaleTag);

	static iser::CArchiveTag indexTag("Index", "Index of found model");
	retVal = retVal && archive.BeginTag(indexTag);
	retVal = retVal && archive.Process(m_index);
	retVal = retVal && archive.EndTag(indexTag);

	static iser::CArchiveTag idTag("Id", "Id of found model");
	retVal = retVal && archive.BeginTag(idTag);
	retVal = retVal && archive.Process(m_id);
	retVal = retVal && archive.EndTag(idTag);

	return retVal;
}


} // namespace iipr


