// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef icam_CBitmapJoinerCompBase_included
#define icam_CBitmapJoinerCompBase_included


// ACF includes
#include <ilog/TLoggerCompWrap.h>
#include <iimg/IBitmap.h>


namespace icam
{


/**
	Joins multiple bitmaps into single one.
	Supports grid layout.
*/
class CBitmapJoinerCompBase: public ilog::CLoggerComponentBase
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CBitmapJoinerCompBase);
		I_ASSIGN(m_numberOfColumnsAttrPtr, "NumberOfColumns", "Number of columns (or, 1 = join vertically, 0 or less = horizontally)", true, 0);
		I_ASSIGN(m_bitmapFactoryPtr, "BitmapFactory", "Bitmap factory", true, "Bitmap");
	I_END_COMPONENT;

protected:
	bool PrepareSnapResult(int imageCount, istd::IChangeable* outputPtr);
	bool FinalizeSnapResult();
	const iimg::IBitmap* GetResultBitmap() const
	{
		return m_resultPtr;
	}

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

protected:
	struct ImageInfo 
	{
		ImageInfo()
		{
			isSnapOk = false;
		}

		bool isSnapOk;
		iimg::IBitmapSharedPtr bitmapPtr;
	};

	typedef QVector<ImageInfo> ImageList;
	ImageList m_snapList;

private:
	iimg::IBitmap* m_resultPtr;

	I_ATTR(int, m_numberOfColumnsAttrPtr);
	I_FACT(iimg::IBitmap, m_bitmapFactoryPtr);
};


} // namespace icam


#endif // !icam_CBitmapJoinerCompBase_included
