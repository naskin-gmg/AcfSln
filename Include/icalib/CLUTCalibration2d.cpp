#include <icalib/CLUTCalibration2d.h>


// ACF includes
#include <imod/TModelWrap.h>
#include <istd/CChangeNotifier.h>
#include <imath/CVarMatrix.h>


namespace icalib
{

// static constants
static const istd::IChangeable::ChangeSet s_calibrationChangedChangeSet(i2d::ICalibration2d::CF_CALIBRATION_CHANGED, "Calibration changed");
static const istd::IChangeable::ChangeSet s_areaChangedChangeSet(i2d::ICalibration2d::CF_AREA_CHANGED, "Calibration area changed");
static const istd::IChangeable::ChangeSet s_unitsChangedChangeSet(i2d::ICalibration2d::CF_UNITS_CHANGED, "Calibration units changed");


// public methods

CLUTCalibration2d::CLUTCalibration2d() : m_argumentAreaPtr(NULL), m_resultAreaPtr(NULL), m_argumentUnitInfoPtr(NULL), m_resultUnitInfoPtr(NULL)
{
}


const iimg::CGeneralBitmap& CLUTCalibration2d::GetLut() const
{
	return m_lut;
}

void CLUTCalibration2d::SetLut(const iimg::IBitmap& lut)
{
	if (lut.GetPixelFormat() != iimg::IBitmap::PF_XY32) {
		return;
	}

	istd::CChangeNotifier notifier(this, &s_areaChangedChangeSet);

	m_lut.CopyFrom(lut);
	m_metricPoints.clear();

	for (int j = 0; j < m_lut.GetImageSize().GetY(); ++j) {
		const float* lutLinePtr = static_cast<const float*>(m_lut.GetLinePtr(j));
		for (int i = 0; i < m_lut.GetImageSize().GetX(); ++i) {
			float x = lutLinePtr[2 * i];
			float y = lutLinePtr[2 * i + 1];
			if (std::isnan(x) || std::isnan(y)) {
				continue;
			}

			MetricPoint p;
			p.pixel.SetX(i);
			p.pixel.SetY(j);
			p.point.SetX(x);
			p.point.SetY(y);
			m_metricPoints.emplace_back(p);
		}
	}

	m_metricPointsTree.MakeTree(
		[](size_t i) { return i; }, m_metricPoints.size(),
		[this](const size_t& p, uint8_t i) { return m_metricPoints[p].point[i]; },
		[this](const std::array<double, 2>& a, size_t b) {
			const i2d::CVector2d& pb = m_metricPoints[b].point;
			return sqrt((a[0] - pb[0]) * (a[0] - pb[0]) + (a[1] - pb[1])*(a[1] - pb[1]));
		}
	);
}


void CLUTCalibration2d::SetArgumentArea(const i2d::CRectangle* areaPtr)
{
	if (areaPtr != m_argumentAreaPtr){
		istd::CChangeNotifier notifier(this, &s_areaChangedChangeSet);
		Q_UNUSED(notifier);

		m_argumentAreaPtr = areaPtr;
	}
}


void CLUTCalibration2d::SetResultArea(const i2d::CRectangle* areaPtr)
{
	if (areaPtr != m_resultAreaPtr){
		istd::CChangeNotifier notifier(this, &s_areaChangedChangeSet);
		Q_UNUSED(notifier);

		m_resultAreaPtr = areaPtr;
	}
}


void CLUTCalibration2d::SetArgumentUnitInfo(const imath::IUnitInfo* unitInfoPtr)
{
	if (unitInfoPtr != m_argumentUnitInfoPtr){
		istd::CChangeNotifier notifier(this, &s_unitsChangedChangeSet);
		Q_UNUSED(notifier);

		m_argumentUnitInfoPtr = unitInfoPtr;
	}
}


void CLUTCalibration2d::SetResultUnitInfo(const imath::IUnitInfo* unitInfoPtr)
{
	if (unitInfoPtr != m_resultUnitInfoPtr){
		istd::CChangeNotifier notifier(this, &s_unitsChangedChangeSet);
		Q_UNUSED(notifier);

		m_resultUnitInfoPtr = unitInfoPtr;
	}
}


// reimplemented (i2d::ICalibration2d)

const i2d::CRectangle* CLUTCalibration2d::GetArgumentArea() const
{
	return m_argumentAreaPtr;
}


const i2d::CRectangle* CLUTCalibration2d::GetResultArea() const
{
	return m_resultAreaPtr;
}


const imath::IUnitInfo* CLUTCalibration2d::GetArgumentUnitInfo() const
{
	return m_argumentUnitInfoPtr;
}


const imath::IUnitInfo* CLUTCalibration2d::GetResultUnitInfo() const
{
	return m_resultUnitInfoPtr;
}


istd::TUniqueInterfacePtr<i2d::ICalibration2d> CLUTCalibration2d::CreateCombinedCalibration(const i2d::ITransformation2d& /*transformation*/) const
{
	return nullptr;
}


// reimplemented (i2d::ITransformation2d)

int CLUTCalibration2d::GetTransformationFlags() const
{
	return TF_FORWARD | TF_INVERTED;
}


bool CLUTCalibration2d::GetDistance(const i2d::CVector2d& /*origPos1*/, const i2d::CVector2d& /*origPos2*/, double& /*result*/, ExactnessMode /*mode*/) const
{
	return false;
}


bool CLUTCalibration2d::GetPositionAt(const i2d::CVector2d& origPosition, i2d::CVector2d& result, ExactnessMode /*mode*/) const
{
	std::array<double, 2> p = { origPosition[0], origPosition[1] };

	size_t nearestIndex;
	double dist;
	bool found = m_metricPointsTree.Nearest(p, nearestIndex, dist);

	if (!found) {
		return false;
	}

	const int currPixelX = m_metricPoints[nearestIndex].pixel.GetX();
	const int currPixelY = m_metricPoints[nearestIndex].pixel.GetY();

	int pixelsX[9] = { currPixelX - 1, currPixelX, currPixelX + 1, currPixelX - 1, currPixelX, currPixelX + 1, currPixelX - 1, currPixelX, currPixelX + 1 };
	int pixelsY[9] = { currPixelY - 1, currPixelY - 1, currPixelY - 1, currPixelY, currPixelY, currPixelY, currPixelY + 1, currPixelY + 1, currPixelY + 1 };
	const istd::CIndex2d lutSize = m_lut.GetImageSize();

	for (int i = 0; i < 9; ++i) {
		if (pixelsX[i] < 0 || pixelsY[i] < 0 || pixelsX[i] >= lutSize.GetX() || pixelsY[i] >= lutSize.GetY()) {
			return false;
		}
	}

	imath::CVarMatrix matrix(istd::CIndex2d(9, 9));

	imath::CVarMatrix xMatrix(istd::CIndex2d(1, 9));
	imath::CVarMatrix yMatrix(istd::CIndex2d(1, 9));
	for (int i = 0; i < 9; ++i) {
		const double x = ((float*)m_lut.GetLinePtr(pixelsY[i]))[2 * pixelsX[i]];
		const double y = ((float*)m_lut.GetLinePtr(pixelsY[i]))[2 * pixelsX[i] + 1];

		matrix.SetAt(istd::CIndex2d(0, i), 1);
		matrix.SetAt(istd::CIndex2d(1, i), x);
		matrix.SetAt(istd::CIndex2d(2, i), x * x);
		matrix.SetAt(istd::CIndex2d(3, i), y);
		matrix.SetAt(istd::CIndex2d(4, i), x * y);
		matrix.SetAt(istd::CIndex2d(5, i), x * x * y);
		matrix.SetAt(istd::CIndex2d(6, i), y * y);
		matrix.SetAt(istd::CIndex2d(7, i), x * y * y);
		matrix.SetAt(istd::CIndex2d(8, i), x * x * y * y);

		xMatrix.SetAt(istd::CIndex2d(0, i), pixelsX[i]);
		yMatrix.SetAt(istd::CIndex2d(0, i), pixelsY[i]);
	}

	imath::CVarMatrix polynomialCoeffMartixX;
	if (!matrix.GetSolvedLSP(xMatrix, polynomialCoeffMartixX)) {
		return false;
	}

	imath::CVarMatrix polynomialCoeffMartixY;
	if (!matrix.GetSolvedLSP(yMatrix, polynomialCoeffMartixY)) {
		return false;
	}

	imath::CVarVector polynomialCoeffX;
	polynomialCoeffMartixX.GetColumnVector(0, polynomialCoeffX);
	imath::CVarVector polynomialCoeffY;
	polynomialCoeffMartixY.GetColumnVector(0, polynomialCoeffY);

	const double resultX = polynomialCoeffX[0] +
		polynomialCoeffX[1] * p[0] +
		polynomialCoeffX[2] * p[0] * p[0] +
		polynomialCoeffX[3] * p[1] +
		polynomialCoeffX[4] * p[0] * p[1] +
		polynomialCoeffX[5] * p[0] * p[0] * p[1] +
		polynomialCoeffX[6] * p[1] * p[1] +
		polynomialCoeffX[7] * p[0] * p[1] * p[1] +
		polynomialCoeffX[8] * p[0] * p[0] * p[1] * p[1];

	const double resultY = polynomialCoeffY[0] +
		polynomialCoeffY[1] * p[0] +
		polynomialCoeffY[2] * p[0] * p[0] +
		polynomialCoeffY[3] * p[1] +
		polynomialCoeffY[4] * p[0] * p[1] +
		polynomialCoeffY[5] * p[0] * p[0] * p[1] +
		polynomialCoeffY[6] * p[1] * p[1] +
		polynomialCoeffY[7] * p[0] * p[1] * p[1] +
		polynomialCoeffY[8] * p[0] * p[0] * p[1] * p[1];

	result.SetX(resultX);
	result.SetY(resultY);

	return true;
}


bool CLUTCalibration2d::GetInvPositionAt(const i2d::CVector2d& transfPosition, i2d::CVector2d& result, ExactnessMode /*mode*/) const
{
	if (m_lut.GetPixelFormat() != iimg::IBitmap::PF_XY32) {
		return false;
	}

	const istd::CIndex2d lutSize = m_lut.GetImageSize();

	if (transfPosition.GetX() < 0 || transfPosition.GetY() < 0 ||
		transfPosition.GetX() >= lutSize.GetX() || transfPosition.GetY() >= lutSize.GetY()) {
		return false;
	}

	const int sourceX = int(transfPosition.GetX());
	const double alphaX = transfPosition.GetX() - sourceX;
	const int sourceY = int(transfPosition.GetY());
	const double alphaY = transfPosition.GetY() - sourceY;

	const float* linePtr = (const float*)m_lut.GetLinePtr(sourceY);
	const float* line1Ptr = (const float*)m_lut.GetLinePtr(sourceY + 1);
	const int lineOffset = sourceX * 2;

	float pixel11ValueX = linePtr[lineOffset];
	float pixel11ValueY = linePtr[lineOffset + 1];
	float pixel12ValueX = linePtr[lineOffset + 2];
	float pixel12ValueY = linePtr[lineOffset + 3];
	float pixel21ValueX = line1Ptr[lineOffset];
	float pixel21ValueY = line1Ptr[lineOffset + 1];
	float pixel22ValueX = line1Ptr[lineOffset + 2];
	float pixel22ValueY = line1Ptr[lineOffset + 3];

	if (std::isnan(pixel11ValueX) || std::isnan(pixel11ValueY) ||
		std::isnan(pixel12ValueX) || std::isnan(pixel12ValueY) ||
		std::isnan(pixel21ValueX) || std::isnan(pixel21ValueY) ||
		std::isnan(pixel22ValueX) || std::isnan(pixel22ValueY)
	) {
		return false;
	}


	pixel11ValueX *= (1 - alphaX);
	pixel12ValueX *= alphaX;
	pixel12ValueX += pixel11ValueX;
	pixel12ValueX *= (1 - alphaY);
	pixel21ValueX *= (1 - alphaX);
	pixel22ValueX *= alphaX;
	pixel22ValueX += pixel21ValueX;
	pixel22ValueX *= alphaY;
	pixel22ValueX += pixel12ValueX;

	pixel11ValueY *= (1 - alphaX);
	pixel12ValueY *= alphaX;
	pixel12ValueY += pixel11ValueY;
	pixel12ValueY *= (1 - alphaY);
	pixel21ValueY *= (1 - alphaX);
	pixel22ValueY *= alphaX;
	pixel22ValueY += pixel21ValueY;
	pixel22ValueY *= alphaY;
	pixel22ValueY += pixel12ValueY;

	result.SetX(pixel22ValueX);
	result.SetY(pixel22ValueY);

	return true;
}


bool CLUTCalibration2d::GetLocalTransform(const i2d::CVector2d& /*origPosition*/, i2d::CAffine2d& /*result*/, ExactnessMode /*mode*/) const
{
	return false;
}


bool CLUTCalibration2d::GetLocalInvTransform(const i2d::CVector2d& /*transfPosition*/, i2d::CAffine2d& /*result*/, ExactnessMode /*mode*/) const
{
	return false;
}


bool CLUTCalibration2d::GetInvValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	return GetInvPositionAt(argument, result);
}


i2d::CVector2d CLUTCalibration2d::GetInvValueAt(const i2d::CVector2d& argument) const
{
	i2d::CVector2d result;

	if (GetInvValueAt(argument, result)) {
		return result;
	}

	return i2d::CVector2d();
}


bool CLUTCalibration2d::GetValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	return GetPositionAt(argument, result);
}


i2d::CVector2d CLUTCalibration2d::GetValueAt(const i2d::CVector2d& argument) const
{
	i2d::CVector2d result;

	if (GetValueAt(argument, result)) {
		return result;
	}

	return i2d::CVector2d();
}


// reimplemented (istd::IChangeable)

int CLUTCalibration2d::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CLUTCalibration2d::CopyFrom(const istd::IChangeable& object, CompatibilityMode mode)
{
	const CLUTCalibration2d* calibrationPtr = dynamic_cast<const CLUTCalibration2d*>(&object);
	if (calibrationPtr != NULL){
		istd::CChangeNotifier notifier(this);
		
		SetLut(calibrationPtr->GetLut());

		if (mode == CM_WITH_REFS){
			m_argumentUnitInfoPtr = calibrationPtr->m_argumentUnitInfoPtr;
			m_resultUnitInfoPtr = calibrationPtr->m_resultUnitInfoPtr;
		}

		return true;
	}

	if ((mode == CM_STRICT) || (mode == CM_CONVERT)){	// we cannot convert different units, we do than strict check
		if ((m_argumentUnitInfoPtr != NULL) || (m_resultUnitInfoPtr != NULL)){
			return false;
		}
	}

	return true;
}


istd::IChangeableUniquePtr CLUTCalibration2d::CloneMe(CompatibilityMode /*mode*/) const
{
	CLUTCalibration2d* result = new CLUTCalibration2d();
	result->SetArgumentArea(m_argumentAreaPtr);
	result->SetArgumentUnitInfo(m_argumentUnitInfoPtr);
	result->SetResultArea(m_argumentAreaPtr);
	result->SetResultUnitInfo(m_argumentUnitInfoPtr);
	result->SetLut(m_lut);

	return result;
}



bool CLUTCalibration2d::ResetData(CompatibilityMode /*mode*/)
{
	m_lut.ResetImage();

	return true;
}

bool CLUTCalibration2d::Serialize(iser::IArchive& archive)
{
	bool result = true;

	if (archive.IsStoring()) {
		m_lut.Serialize(archive);
	}
	else {
		iimg::CGeneralBitmap lut;
		lut.Serialize(archive);
		SetLut(lut);
	}

	return result;
}


} // namespace icalib


