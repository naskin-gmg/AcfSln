#ifndef ialgo_CHoughSpace2d_included
#define ialgo_CHoughSpace2d_included


// Qt includes
#include <QtCore/QMultiMap>

// ACF includes
#include <i2d/CVector2d.h>
#include <iimg/CGeneralBitmap.h>

// ACF-Solutions includes
#include <ialgo/TIHoughSpace.h>


namespace ialgo
{


/**
	Hough space used for 2d Hough transformations.
*/
class CHoughSpace2d:
			public iimg::CGeneralBitmap,
			virtual public TIHoughSpace<2>
{
public:
	typedef iimg::CGeneralBitmap BaseClass;

	typedef QMultiMap<double, i2d::CVector2d> WeightToHoughPosMap;
	class StdConsumer: public ResultsConsumer
	{
	public:
		/**
			Construct the result processor.
			\param	maxPoints				maximal number of points beeing in result set.
			\param	maxConsideredPoints		maximal number of points stored internally.
											Setting this number to smaller value speeds up the processing.
			\param	minDistance				Minimal distance between the points.
											Please note that the neighbourshood will be analysed at end.
		*/
		StdConsumer(int maxPoints, int maxConsideredPoints, double minDistance, double minMaxRatio);

		// reimplemented (ialgo::TIHoughSpace<2>::ResultsConsumer)
		virtual QList<int> GetSupportedNeghboursCount() const;
		virtual void OnProcessingBegin(
					const TIHoughSpace<2, double>& space,
					const double& minValue);
		virtual void OnProcessingEnd(const TIHoughSpace<2, double>& space);
		virtual bool OnMaximumFound(
					const TIHoughSpace<2, double>& space,
					const istd::TIndex<2>& position,
					const double& value,
					const double* neghboursPtr,
					int neghboursCount,
					double& minValue);

		WeightToHoughPosMap positions;

	private:
		int m_maxPoints;
		int m_maxConsideredPoints;
		double m_minDistance;
		double m_minMaxRatio;

		double m_maxValue;
	};

	CHoughSpace2d();
	CHoughSpace2d(const istd::CIndex2d& size, bool isWrappedX = false, bool isWrappedY = false);

	bool CreateHoughSpace(
				const istd::CIndex2d& size,
				bool isWrappedX = false,
				bool isWrappedY = false,
				bool isFloatSpace = false);

	/**
		Apply some operation to each element.
	*/
	template <typename Operation>
	void ApplyOperation(Operation operation);

	/**
		Combine this space with some other space.
	*/
	template <typename Operation>
	void CombineWithSpace(const CHoughSpace2d& space, Operation operation);

	// reimplemented (ialgo::TIHoughSpace<2>)
	virtual bool CreateHoughSpace(const istd::TIndex<2>& size, const double& initValue = 0);
	virtual bool IsDimensionWrapped(int dimensionIndex) const;
	virtual void SetDimensionWrapped(int dimensionIndex, bool state);
	virtual void IncreaseValueAt(const imath::TVector<2>& position, double value);
	virtual void SmoothHoughSpace(const istd::TIndex<2>& iterations);
	virtual bool AnalyseHoughSpace(
				const double& minValue,
				ResultsConsumer& resultProcessor);
	virtual bool ExtractToBitmap(iimg::IBitmap& bitmap) const;
	virtual bool GetSpacePosition(const imath::TVector<2>& position, imath::TVector<2>& result) const;
	virtual double GetDistance(const imath::TVector<2>& position1, const imath::TVector<2>& position2) const;
	virtual double GetDistance2(const imath::TVector<2>& position1, const imath::TVector<2>& position2) const;

	// reimplemented (iimg::CGeneralBitmap)
	virtual bool CreateBitmap(PixelFormat pixelFormat, const istd::CIndex2d& size, int pixelBitsCount = 0, int componentsCount = 0);
	virtual bool CreateBitmap(PixelFormat pixelFormat, const istd::CIndex2d& size, void* dataPtr, bool releaseFlag, int linesDifference = 0);

private:
	bool m_isWrapped[2];
};


// inline methods

inline double CHoughSpace2d::GetDistance(const imath::TVector<2>& position1, const imath::TVector<2>& position2) const
{
	return qSqrt(GetDistance2(position1, position2));
}


inline double CHoughSpace2d::GetDistance2(const imath::TVector<2>& position1, const imath::TVector<2>& position2) const
{
	istd::CIndex2d spaceSize = BaseClass::GetImageSize();

	i2d::CVector2d diff = position2 - position1;
	if (m_isWrapped[0]){
		double offset = spaceSize.GetX() * 0.5;
		diff.SetX(std::fmod(diff.GetX() + offset + spaceSize.GetX(),  spaceSize.GetX()) - offset);
	}

	if (m_isWrapped[1]){
		double offset = spaceSize.GetY() * 0.5;
		diff.SetY(std::fmod(diff.GetY() + offset + spaceSize.GetY(),  spaceSize.GetY()) - offset);
	}

	return diff.GetLength2();
}


// template methods

template <typename Operation>
void CHoughSpace2d::ApplyOperation(Operation operation)
{

	istd::CIndex2d size = BaseClass::GetImageSize();

	for (int y = 0; y < size.GetY(); ++y){
		if (BaseClass::GetPixelFormat() == PF_FLOAT32){
			float* linePtr = (float*)BaseClass::GetLinePtr(y);
			for (int x = 0; x < size.GetX(); ++x){
				linePtr[x] = operation(linePtr[x]);
			}
		}
		else{
			Q_ASSERT(GetPixelFormat() == PF_GRAY32);

			quint32* linePtr = (quint32*)BaseClass::GetLinePtr(y);
			for (int x = 0; x < size.GetX(); ++x){
				linePtr[x] = operation(linePtr[x]);
			}
		}
	}
}


template <typename Operation>
void CHoughSpace2d::CombineWithSpace(const CHoughSpace2d& space, Operation operation)
{
	istd::CIndex2d size = BaseClass::GetImageSize();
	istd::CIndex2d spaceSize = space.GetImageSize();

	istd::CIndex2d commonSize(qMin(size.GetX(), spaceSize.GetX()), qMin(size.GetY(), spaceSize.GetY()));
	for (int y = 0; y < commonSize.GetY(); ++y){
		if (BaseClass::GetPixelFormat() == PF_FLOAT32){
			float* linePtr = (float*)BaseClass::GetLinePtr(y);
			const float* spaceLinePtr = (const float*)space.GetLinePtr(y);
			for (int x = 0; x < commonSize.GetX(); ++x){
				linePtr[x] = operation(linePtr[x], spaceLinePtr[x]);
			}
		}
		else{
			Q_ASSERT(GetPixelFormat() == PF_GRAY32);

			quint32* linePtr = (quint32*)BaseClass::GetLinePtr(y);
			const quint32* spaceLinePtr = (const quint32*)space.GetLinePtr(y);
			for (int x = 0; x < commonSize.GetX(); ++x){
				linePtr[x] = operation(linePtr[x], spaceLinePtr[x]);
			}
		}
	}
}


} // namespace ialgo


#endif // !ialgo_CHoughSpace2d_included


