#include "iipr/CSearchFeature.h"


// ACF includes
#include "istd/TDelPtr.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


// public methods

CSearchFeature::CSearchFeature()
	:BaseClass(0.0),
	m_scale(i2d::CVector2d()),
	m_angle(0.0),
	m_index(0)
{
}


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

	imath::CVarVector values(6, 0.0);

	values[0] = weight;
	values[1] = position.GetX();
	values[2] = position.GetY();
	values[3] = scale.GetX();
	values[4] = scale.GetY();
	values[5] = angle;

	SetValues(values);
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


void CSearchFeature::SetId(QString id)
{
	if (m_id != id){
		istd::CChangeNotifier notifier(this);
		
		m_id = id;
	}
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

	default:
		return false;
	}
}


imath::CVarVector CSearchFeature::GetComponentValue(CSearchFeature::ValueTypeId valueTypeId) const
{
	imath::CVarVector result;

	switch (valueTypeId){
	case VTI_AUTO:
		return BaseClass::GetComponentValue(VTI_AUTO);

	case VTI_POSITION:
		result.SetElementsCount(2, 0);
		result[0] = GetPosition().GetX();
		result[1] = GetPosition().GetY();
		break;

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

	default:
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


// reimplemented (istd::IChangeable)

bool CSearchFeature::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CSearchFeature* objectPtr = dynamic_cast<const CSearchFeature*>(&object);

	if (objectPtr != NULL){
		istd::CChangeNotifier notifier(this);

		m_scale = objectPtr->GetScale();
		m_angle = objectPtr->GetAngle();
		m_index = objectPtr->GetIndex();
		m_id = objectPtr->GetId();

		BaseClass::CopyFrom(object, mode);

		return true;
	}

	return false;
}

istd::IChangeable* CSearchFeature::CloneMe(CompatibilityMode mode) const
{
	istd::TDelPtr<CSearchFeature> retVal(new CSearchFeature);

	if (retVal->CopyFrom(*this, mode)){
		return retVal.PopPtr();
	}

	return NULL;
}


} // namespace iipr


