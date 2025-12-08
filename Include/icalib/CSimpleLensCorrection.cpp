#include <icalib/CSimpleLensCorrection.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <i2d/CAffine2d.h>


namespace icalib
{


// static constants
static const iser::CArchiveTag s_distortionFactorTag("DistFactor", "Lens distortion factor", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_scaleFactorTag("Scale", "Lens scale factor", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_opticalCenterTag("OpticalCenter", "Position of optical center", iser::CArchiveTag::TT_GROUP);


CSimpleLensCorrection::CSimpleLensCorrection()
:	m_opticalCenter(i2d::CVector2d::GetZero()),
	m_distortionFactor(0),
	m_scaleFactor(1)
{
}


CSimpleLensCorrection::CSimpleLensCorrection(const i2d::CVector2d& opticalCenter, double distortionFactor, double scaleFactor)
:	m_opticalCenter(opticalCenter),
	m_distortionFactor(distortionFactor),
	m_scaleFactor(scaleFactor)
{
}


/**
	Reset this calibration, set to be identity transform.
*/
void CSimpleLensCorrection::Reset(const i2d::CVector2d& opticalCenter, double distortionFactor, double scaleFactor)
{
	m_opticalCenter = opticalCenter;
	m_distortionFactor = distortionFactor;
	m_scaleFactor = scaleFactor;
}


const i2d::CVector2d& CSimpleLensCorrection::GetOpticalCenter() const
{
	return m_opticalCenter;
}


void CSimpleLensCorrection::SetOpticalCenter(const i2d::CVector2d& center)
{
	if (center != m_opticalCenter){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		m_opticalCenter = center;
	}
}


double CSimpleLensCorrection::GetDistortionFactor() const
{
	return m_distortionFactor;
}


void CSimpleLensCorrection::SetDistortionFactor(double factor)
{
	if (factor != m_distortionFactor){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		m_distortionFactor = factor;
	}
}


double CSimpleLensCorrection::GetScaleFactor() const
{
	return m_scaleFactor;
}


void CSimpleLensCorrection::SetScaleFactor(double factor)
{
	Q_ASSERT(m_scaleFactor != 0);

	if (factor != m_scaleFactor){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		m_scaleFactor = factor;
	}
}


bool CSimpleLensCorrection::operator==(const CSimpleLensCorrection& calib) const
{
	return		(m_opticalCenter == calib.m_opticalCenter) &&
				(m_distortionFactor == calib.m_distortionFactor) &&
				(m_scaleFactor == calib.m_scaleFactor);
}


// reimplemented (i2d::ICalibration2d)

const i2d::CRectangle* CSimpleLensCorrection::GetArgumentArea() const
{
	return NULL;
}


const i2d::CRectangle* CSimpleLensCorrection::GetResultArea() const
{
	return NULL;
}


const imath::IUnitInfo* CSimpleLensCorrection::GetArgumentUnitInfo() const
{
	return NULL;
}


const imath::IUnitInfo* CSimpleLensCorrection::GetResultUnitInfo() const
{
	return NULL;
}


istd::TUniqueInterfacePtr<i2d::ICalibration2d> CSimpleLensCorrection::CreateCombinedCalibration(const ITransformation2d& /*transformation*/) const
{
	return NULL;
}


// reimplemented (i2d::ITransformation2d)

int CSimpleLensCorrection::GetTransformationFlags() const
{
	int retVal = TF_FORWARD | TF_INVERTED | TF_INJECTIVE | TF_SURJECTIVE | TF_CONTINUES;

	if (m_distortionFactor == 0){
		retVal |= TF_PRESERVE_DISTANCE | TF_PRESERVE_ANGLE | TF_AFFINE;
	}

	if (m_opticalCenter.IsNull()){
		retVal |= TF_PRESERVE_NULL;
	}

	return retVal;
}


bool CSimpleLensCorrection::GetDistance(
			const i2d::CVector2d& origPos1,
			const i2d::CVector2d& origPos2,
			double& result,
			ExactnessMode /*mode*/) const
{
	i2d::CVector2d log1;
	i2d::CVector2d log2;

	if (		GetPositionAt(origPos1, log1) &&
				GetPositionAt(origPos2, log2)){
		result = log1.GetDistance(log2);

		return true;
	}

	return false;
}


bool CSimpleLensCorrection::GetPositionAt(
			const i2d::CVector2d& origPosition,
			i2d::CVector2d& result,
			ExactnessMode /*mode*/) const
{
	double distance = origPosition.GetLength();
	if (distance * 2 * m_distortionFactor * m_scaleFactor < -m_scaleFactor){
		return false;
	}

	double scaleFactor = (1 + m_distortionFactor * distance) * m_scaleFactor;

	result = m_opticalCenter + origPosition * scaleFactor;

	return true;
}


bool CSimpleLensCorrection::GetInvPositionAt(
			const i2d::CVector2d& transfPosition,
			i2d::CVector2d& result,
			ExactnessMode /*mode*/) const
{
	i2d::CVector2d normPos = (transfPosition - m_opticalCenter) / m_scaleFactor;

	double distance = normPos.GetLength();

	if (qFabs(m_distortionFactor) > I_BIG_EPSILON){
		double delta = 1 + 4 * distance * m_distortionFactor;
		if (delta >= 0){
			double shouldDistance = (-1 + qSqrt(delta)) / (2 * m_distortionFactor);

			result = normPos.GetNormalized(shouldDistance);

			return true;
		}

		return false;
	}
	else{
		result = normPos;

		return true;
	}
}


bool CSimpleLensCorrection::GetLocalTransform(
			const i2d::CVector2d& origPosition,
			i2d::CAffine2d& result,
			ExactnessMode /*mode*/) const
{
	double distance = origPosition.GetLength();
	if (distance * 2 * m_distortionFactor * m_scaleFactor < -m_scaleFactor){
		return false;
	}

	double scaleFactor = (1 + m_distortionFactor * distance) * m_scaleFactor;

	result.SetTranslation(m_opticalCenter + origPosition * (scaleFactor - 1));
	result.SetDeformMatrix(i2d::CMatrix2d::GetIdentity() * scaleFactor);	// TODO: implement it correctly, it is simple approximation only

	return true;
}


bool CSimpleLensCorrection::GetLocalInvTransform(
			const i2d::CVector2d& transfPosition,
			i2d::CAffine2d& result,
			ExactnessMode /*mode*/) const
{
	i2d::CVector2d normPos = (transfPosition - m_opticalCenter) / m_scaleFactor;

	double distance = normPos.GetLength();

	double delta = 1 + 4 * distance * m_distortionFactor;
	if (delta >= 0){
		double shouldDistance = (-1 + qSqrt(delta)) * 0.5;

		if (shouldDistance >= I_BIG_EPSILON){
			result.SetTranslation(normPos.GetNormalized(shouldDistance) - transfPosition / m_scaleFactor);
			result.SetDeformMatrix(i2d::CMatrix2d::GetIdentity());	// TODO: implement it correctly
		}
		else{
			result.SetTranslation(-m_opticalCenter / m_scaleFactor);
			result.SetDeformMatrix(i2d::CMatrix2d::GetIdentity());
		}

		return true;
	}

	return false;
}


// reimplemented (imath::TISurjectFunction)

bool CSimpleLensCorrection::GetInvValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	return GetInvPositionAt(argument, result);
}


i2d::CVector2d CSimpleLensCorrection::GetInvValueAt(const i2d::CVector2d& argument) const
{
	i2d::CVector2d retVal;
	
	GetInvValueAt(argument, retVal);

	return retVal;
}


// reimplemented (imath::TIMathFunction)

bool CSimpleLensCorrection::GetValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	return GetPositionAt(argument, result);
}


i2d::CVector2d CSimpleLensCorrection::GetValueAt(const i2d::CVector2d& argument) const
{
	i2d::CVector2d retVal;
	
	GetValueAt(argument, retVal);

	return retVal;
}
	
	
// reimplemented (iser::ISerializable)

bool CSimpleLensCorrection::Serialize(iser::IArchive& archive)
{
	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(s_distortionFactorTag);
	retVal = retVal && archive.Process(m_distortionFactor);
	retVal = retVal && archive.EndTag(s_distortionFactorTag);

	retVal = retVal && archive.BeginTag(s_scaleFactorTag);
	retVal = retVal && archive.Process(m_scaleFactor);
	retVal = retVal && archive.EndTag(s_scaleFactorTag);

	retVal = retVal && archive.BeginTag(s_opticalCenterTag);
	retVal = retVal && m_opticalCenter.Serialize(archive);
	retVal = retVal && archive.EndTag(s_opticalCenterTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

int CSimpleLensCorrection::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE | SO_COMPARE | SO_RESET;
}


bool CSimpleLensCorrection::CopyFrom(const istd::IChangeable& object, CompatibilityMode /*mode*/)
{
	const CSimpleLensCorrection* calibrationImplPtr = dynamic_cast<const CSimpleLensCorrection*>(&object);
	if (calibrationImplPtr != NULL){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		*this = *calibrationImplPtr;

		return true;
	}

	return false;
}


bool CSimpleLensCorrection::IsEqual(const istd::IChangeable& object) const
{
	const CSimpleLensCorrection* calibrationImplPtr = dynamic_cast<const CSimpleLensCorrection*>(&object);
	if (calibrationImplPtr != NULL){
		return *this == *calibrationImplPtr;
	}

	return false;
}


istd::IChangeableUniquePtr CSimpleLensCorrection::CloneMe(CompatibilityMode /*mode*/) const
{
	return new CSimpleLensCorrection(*this);
}


bool CSimpleLensCorrection::ResetData(CompatibilityMode /*mode*/)
{
	Reset();

	return true;
}


} // namespace icalib


