// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iipr/CPerspCalibFinder.h>


// ACF includes
#include <imath/CVarMatrix.h>


namespace iipr
{


bool CPerspCalibFinder::FindPerspCalib(
			const i2d::CVector2d* nominalPositionsPtr,
			const i2d::CVector2d* foundPositionsPtr,
			int positionsCount,
			icalib::CPerspectiveCalibration2d& result,
			bool allowPerspective,
			bool allowRotation,
			bool allowScale,
			bool allowAnisotropic,
			bool allowTranslation) const
{
	int degsOfFreedom = positionsCount * 2;

	if (allowPerspective && allowRotation && allowScale && allowAnisotropic && allowTranslation && (degsOfFreedom >= 8)){	// try perspective transformation
		// |   u_i     v_i      1       0       0       0   -u_i*x_i  -v_i*x_i  | |r_0_0  r_0_1  r_0_2  r_1_0  r_1_1  r_1_2  r_2_0  r_2_1|^T = |x_i|
		// |    0       0       0      u_i     v_i      1   -u_i*y_i  -v_i*y_i  | |r_0_0  r_0_1  r_0_2  r_1_0  r_1_1  r_1_2  r_2_0  r_2_1|^T = |y_i|
		//
		// where
		//			{u_i,v_i} is i-th nominal position
		//			{x_i,y_i} is i-th found bitmap position
		//
		//			    | r_0_0  r_0_1  r_0_2 |
		//			R = | r_1_0  r_1_1  r_1_2 |, is matrix representation of perspective function.
		//			    | r_2_0  r_2_1    1   |

		imath::CVarMatrix A(istd::CIndex2d(8, degsOfFreedom));
		imath::CVarMatrix x(istd::CIndex2d(1, degsOfFreedom));

		for (int i = 0; i < positionsCount; ++i){
			const i2d::CVector2d& nomPos = nominalPositionsPtr[i];
			const i2d::CVector2d& foundPos = foundPositionsPtr[i];

			A.SetAt(istd::CIndex2d(0, i * 2 + 0), nomPos.GetX());
			A.SetAt(istd::CIndex2d(1, i * 2 + 0), nomPos.GetY());
			A.SetAt(istd::CIndex2d(2, i * 2 + 0), 1);
			A.SetAt(istd::CIndex2d(3, i * 2 + 0), 0);
			A.SetAt(istd::CIndex2d(4, i * 2 + 0), 0);
			A.SetAt(istd::CIndex2d(5, i * 2 + 0), 0);
			A.SetAt(istd::CIndex2d(6, i * 2 + 0), -nomPos.GetX() * foundPos.GetX());
			A.SetAt(istd::CIndex2d(7, i * 2 + 0), -nomPos.GetY() * foundPos.GetX());
			x.SetAt(istd::CIndex2d(0, i * 2 + 0), foundPos.GetX());

			A.SetAt(istd::CIndex2d(0, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(1, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(2, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(3, i * 2 + 1), nomPos.GetX());
			A.SetAt(istd::CIndex2d(4, i * 2 + 1), nomPos.GetY());
			A.SetAt(istd::CIndex2d(5, i * 2 + 1), 1);
			A.SetAt(istd::CIndex2d(6, i * 2 + 1), -nomPos.GetX() * foundPos.GetY());
			A.SetAt(istd::CIndex2d(7, i * 2 + 1), -nomPos.GetY() * foundPos.GetY());
			x.SetAt(istd::CIndex2d(0, i * 2 + 1), foundPos.GetY());
		}

		imath::CVarMatrix res(istd::CIndex2d(1, 8));
		if (A.GetSolvedLSP(x, res)){
			i2d::CMatrix2d rotationMatrix(
						res.GetAt(istd::CIndex2d(0, 0)), res.GetAt(istd::CIndex2d(0, 3)),
						res.GetAt(istd::CIndex2d(0, 1)), res.GetAt(istd::CIndex2d(0, 4)));
			i2d::CVector2d translation(res.GetAt(istd::CIndex2d(0, 2)), res.GetAt(istd::CIndex2d(0, 5)));
			i2d::CVector2d perspective(res.GetAt(istd::CIndex2d(0, 6)), res.GetAt(istd::CIndex2d(0, 7)));

			result.SetAffinePart(i2d::CAffine2d(rotationMatrix, translation));
			result.SetPerspAxis(perspective);

			return true;
		}
	}

	if (allowRotation && allowScale && allowAnisotropic && allowTranslation && (degsOfFreedom >= 6)){	// try affine transformation
		// |   u_i     v_i      1       0       0       0   | |r_0_0  r_0_1  r_0_2  r_1_0  r_1_1  r_1_2|^T = |x_i|
		// |    0       0       0      u_i     v_i      1   | |r_0_0  r_0_1  r_0_2  r_1_0  r_1_1  r_1_2|^T = |y_i|
		//
		// where
		//			{u_i,v_i} is i-th nominal position
		//			{x_i,y_i} is i-th found bitmap position
		//
		//			    | r_0_0  r_0_1  r_0_2 |
		//			R = | r_1_0  r_1_1  r_1_2 |, is matrix representation of affine function.
		//			    |   0      0      1   |

		imath::CVarMatrix A(istd::CIndex2d(6, degsOfFreedom));
		imath::CVarMatrix x(istd::CIndex2d(1, degsOfFreedom));

		for (int i = 0; i < positionsCount; ++i){
			const i2d::CVector2d& nomPos = nominalPositionsPtr[i];
			const i2d::CVector2d& foundPos = foundPositionsPtr[i];

			A.SetAt(istd::CIndex2d(0, i * 2 + 0), nomPos.GetX());
			A.SetAt(istd::CIndex2d(1, i * 2 + 0), nomPos.GetY());
			A.SetAt(istd::CIndex2d(2, i * 2 + 0), 1);
			A.SetAt(istd::CIndex2d(3, i * 2 + 0), 0);
			A.SetAt(istd::CIndex2d(4, i * 2 + 0), 0);
			A.SetAt(istd::CIndex2d(5, i * 2 + 0), 0);
			x.SetAt(istd::CIndex2d(0, i * 2 + 0), foundPos.GetX());

			A.SetAt(istd::CIndex2d(0, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(1, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(2, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(3, i * 2 + 1), nomPos.GetX());
			A.SetAt(istd::CIndex2d(4, i * 2 + 1), nomPos.GetY());
			A.SetAt(istd::CIndex2d(5, i * 2 + 1), 1);
			x.SetAt(istd::CIndex2d(0, i * 2 + 1), foundPos.GetY());
		}

		imath::CVarMatrix res(istd::CIndex2d(1, 6));
		if (A.GetSolvedLSP(x, res)){
			i2d::CMatrix2d rotationMatrix(
						res.GetAt(istd::CIndex2d(0, 0)), res.GetAt(istd::CIndex2d(0, 3)),
						res.GetAt(istd::CIndex2d(0, 1)), res.GetAt(istd::CIndex2d(0, 4)));
			i2d::CVector2d translation(res.GetAt(istd::CIndex2d(0, 2)), res.GetAt(istd::CIndex2d(0, 5)));

			result.SetAffinePart(i2d::CAffine2d(rotationMatrix, translation));
			result.SetPerspAxis(i2d::CVector2d::GetZero());

			return true;
		}
	}

	if (allowRotation && allowScale && allowTranslation && (degsOfFreedom >= 4)){	// try affine transformation
		// |   u_i      1     -v_i      0   | | d_x   r_0_2   d_y    r_1_2|^T = |x_i|
		// |   v_i      0      u_i      1   | | d_x   r_0_2   d_y    r_1_2|^T = |y_i|
		//
		// where
		//			{u_i,v_i} is i-th nominal position
		//			{x_i,y_i} is i-th found bitmap position
		//
		//			    |  d_x   -d_y   r_0_2 |
		//			R = |  d_y    d_x   r_1_2 |, is matrix representation of affine function.
		//			    |   0      0      1   |

		imath::CVarMatrix A(istd::CIndex2d(4, degsOfFreedom));
		imath::CVarMatrix x(istd::CIndex2d(1, degsOfFreedom));

		for (int i = 0; i < positionsCount; ++i){
			const i2d::CVector2d& nomPos = nominalPositionsPtr[i];
			const i2d::CVector2d& foundPos = foundPositionsPtr[i];

			A.SetAt(istd::CIndex2d(0, i * 2 + 0), nomPos.GetX());
			A.SetAt(istd::CIndex2d(1, i * 2 + 0), 1);
			A.SetAt(istd::CIndex2d(2, i * 2 + 0), -nomPos.GetY());
			A.SetAt(istd::CIndex2d(3, i * 2 + 0), 0);
			x.SetAt(istd::CIndex2d(0, i * 2 + 0), foundPos.GetX());

			A.SetAt(istd::CIndex2d(0, i * 2 + 1), nomPos.GetY());
			A.SetAt(istd::CIndex2d(1, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(2, i * 2 + 1), nomPos.GetX());
			A.SetAt(istd::CIndex2d(3, i * 2 + 1), 1);
			x.SetAt(istd::CIndex2d(0, i * 2 + 1), foundPos.GetY());
		}

		imath::CVarMatrix res(istd::CIndex2d(1, 4));
		if (A.GetSolvedLSP(x, res)){
			i2d::CMatrix2d rotationMatrix(
						res.GetAt(istd::CIndex2d(0, 0)), res.GetAt(istd::CIndex2d(0, 2)),
						-res.GetAt(istd::CIndex2d(0, 2)), res.GetAt(istd::CIndex2d(0, 0)));
			i2d::CVector2d translation(res.GetAt(istd::CIndex2d(0, 1)), res.GetAt(istd::CIndex2d(0, 3)));

			result.SetAffinePart(i2d::CAffine2d(rotationMatrix, translation));
			result.SetPerspAxis(i2d::CVector2d::GetZero());

			return true;
		}
	}

	if (allowScale && allowTranslation && allowAnisotropic && (degsOfFreedom >= 4)){	// try anisotropic scaling transformation
		// |   u_i     0      1      0   | |s_x  s_y  r_0_2  r_1_2|^T = |x_i|
		// |    0     v_i     0      1   | |s_x  s_y  r_0_2  r_1_2|^T = |y_i|
		//
		// where
		//			{u_i,v_i} is i-th nominal position
		//			{x_i,y_i} is i-th found bitmap position
		//
		//			    |  s_x     0    r_0_2 |
		//			R = |   0     s_y   r_1_2 |, is matrix representation of scaling function with translation.
		//			    |   0      0      1   |

		imath::CVarMatrix A(istd::CIndex2d(4, degsOfFreedom));
		imath::CVarMatrix x(istd::CIndex2d(1, degsOfFreedom));

		for (int i = 0; i < positionsCount; ++i){
			const i2d::CVector2d& nomPos = nominalPositionsPtr[i];
			const i2d::CVector2d& foundPos = foundPositionsPtr[i];

			A.SetAt(istd::CIndex2d(0, i * 2 + 0), nomPos.GetX());
			A.SetAt(istd::CIndex2d(1, i * 2 + 0), 0);
			A.SetAt(istd::CIndex2d(2, i * 2 + 0), 1);
			A.SetAt(istd::CIndex2d(3, i * 2 + 0), 0);
			x.SetAt(istd::CIndex2d(0, i * 2 + 0), foundPos.GetX());

			A.SetAt(istd::CIndex2d(0, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(1, i * 2 + 1), nomPos.GetY());
			A.SetAt(istd::CIndex2d(2, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(3, i * 2 + 1), 1);
			x.SetAt(istd::CIndex2d(0, i * 2 + 1), foundPos.GetY());
		}

		imath::CVarMatrix res(istd::CIndex2d(1, 4));
		if (A.GetSolvedLSP(x, res)){
			i2d::CMatrix2d rotationMatrix(
						res.GetAt(istd::CIndex2d(0, 0)), 0,
						0, res.GetAt(istd::CIndex2d(0, 1)));
			i2d::CVector2d translation(res.GetAt(istd::CIndex2d(0, 2)), res.GetAt(istd::CIndex2d(0, 3)));

			result.SetAffinePart(i2d::CAffine2d(rotationMatrix, translation));
			result.SetPerspAxis(i2d::CVector2d::GetZero());

			return true;
		}
	}

	if (allowScale && allowTranslation && (degsOfFreedom >= 3)){	// try isotropic scaling transformation
		// |   u_i     1      0   | |s  r_0_2  r_1_2|^T = |x_i|
		// |   v_i     0      1   | |s  r_0_2  r_1_2|^T = |y_i|
		//
		// where
		//			{u_i,v_i} is i-th nominal position
		//			{x_i,y_i} is i-th found bitmap position
		//
		//			    |   s      0    r_0_2 |
		//			R = |   0      s    r_1_2 |, is matrix representation of scaling function with translation.
		//			    |   0      0      1   |

		imath::CVarMatrix A(istd::CIndex2d(3, degsOfFreedom));
		imath::CVarMatrix x(istd::CIndex2d(1, degsOfFreedom));

		for (int i = 0; i < positionsCount; ++i){
			const i2d::CVector2d& nomPos = nominalPositionsPtr[i];
			const i2d::CVector2d& foundPos = foundPositionsPtr[i];

			A.SetAt(istd::CIndex2d(0, i * 2 + 0), nomPos.GetX());
			A.SetAt(istd::CIndex2d(1, i * 2 + 0), 1);
			A.SetAt(istd::CIndex2d(2, i * 2 + 0), 0);
			x.SetAt(istd::CIndex2d(0, i * 2 + 0), foundPos.GetX());

			A.SetAt(istd::CIndex2d(0, i * 2 + 1), nomPos.GetY());
			A.SetAt(istd::CIndex2d(1, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(2, i * 2 + 1), 1);
			x.SetAt(istd::CIndex2d(0, i * 2 + 1), foundPos.GetY());
		}

		imath::CVarMatrix res(istd::CIndex2d(1, 3));
		if (A.GetSolvedLSP(x, res)){
			i2d::CMatrix2d rotationMatrix(
						res.GetAt(istd::CIndex2d(0, 0)), 0,
						0, res.GetAt(istd::CIndex2d(0, 0)));
			i2d::CVector2d translation(res.GetAt(istd::CIndex2d(0, 1)), res.GetAt(istd::CIndex2d(0, 2)));

			result.SetAffinePart(i2d::CAffine2d(rotationMatrix, translation));
			result.SetPerspAxis(i2d::CVector2d::GetZero());

			return true;
		}
	}

	if (allowTranslation && (degsOfFreedom >= 2)){	// try translation function
		i2d::CVector2d cummTranslation(0, 0);

		for (int i = 0; i < positionsCount; ++i){
			const i2d::CVector2d& nomPos = nominalPositionsPtr[i];
			const i2d::CVector2d& foundPos = foundPositionsPtr[i];

			cummTranslation += foundPos - nomPos;
		}

		result.SetAffinePart(i2d::CAffine2d(i2d::CMatrix2d::GetIdentity(), cummTranslation / positionsCount));
		result.SetPerspAxis(i2d::CVector2d::GetZero());

		return true;
	}

	return false;
}


bool CPerspCalibFinder::FindPerspCalibWithCorrection(
			const i2d::CVector2d* nominalPositionsPtr,
			const i2d::CVector2d* foundPositionsPtr,
			int positionsCount,
			const i2d::CVector2d& opticalCenter,
			icalib::CPerspectiveCalibration2d& result,
			double& lensCorrFactor,
			bool allowPerspective,
			bool allowRotation,
			bool allowScale,
			bool allowAnisotropic,
			bool allowTranslation) const
{
	int degsOfFreedom = positionsCount * 2;

	lensCorrFactor = 0;

	if (allowPerspective && allowRotation && allowScale && allowAnisotropic && allowTranslation && (degsOfFreedom >= 9)){	// try perspective transformation
		imath::CVarMatrix A(istd::CIndex2d(9, degsOfFreedom));
		imath::CVarMatrix x(istd::CIndex2d(1, degsOfFreedom));

		for (int i = 0; i < positionsCount; ++i){
			const i2d::CVector2d& nomPos = nominalPositionsPtr[i];
			const i2d::CVector2d& foundPos = foundPositionsPtr[i];

			i2d::CVector2d foundDiffVector = foundPos - opticalCenter;
			double foundDist = foundDiffVector.GetLength();

			A.SetAt(istd::CIndex2d(0, i * 2 + 0), nomPos.GetX());
			A.SetAt(istd::CIndex2d(1, i * 2 + 0), nomPos.GetY());
			A.SetAt(istd::CIndex2d(2, i * 2 + 0), 1);
			A.SetAt(istd::CIndex2d(3, i * 2 + 0), 0);
			A.SetAt(istd::CIndex2d(4, i * 2 + 0), 0);
			A.SetAt(istd::CIndex2d(5, i * 2 + 0), 0);
			A.SetAt(istd::CIndex2d(6, i * 2 + 0), -nomPos.GetX() * foundPos.GetX());
			A.SetAt(istd::CIndex2d(7, i * 2 + 0), -nomPos.GetY() * foundPos.GetX());
			A.SetAt(istd::CIndex2d(8, i * 2 + 0), foundDiffVector.GetX() * foundDist);
			x.SetAt(istd::CIndex2d(0, i * 2 + 0), foundPos.GetX());

			A.SetAt(istd::CIndex2d(0, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(1, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(2, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(3, i * 2 + 1), nomPos.GetX());
			A.SetAt(istd::CIndex2d(4, i * 2 + 1), nomPos.GetY());
			A.SetAt(istd::CIndex2d(5, i * 2 + 1), 1);
			A.SetAt(istd::CIndex2d(6, i * 2 + 1), -nomPos.GetX() * foundPos.GetY());
			A.SetAt(istd::CIndex2d(7, i * 2 + 1), -nomPos.GetY() * foundPos.GetY());
			A.SetAt(istd::CIndex2d(8, i * 2 + 1), foundDiffVector.GetY() * foundDist);
			x.SetAt(istd::CIndex2d(0, i * 2 + 1), foundPos.GetY());
		}

		imath::CVarMatrix res(istd::CIndex2d(1, 9));
		if (A.GetSolvedLSP(x, res)){
			i2d::CMatrix2d rotationMatrix(
						res.GetAt(istd::CIndex2d(0, 0)), res.GetAt(istd::CIndex2d(0, 3)),
						res.GetAt(istd::CIndex2d(0, 1)), res.GetAt(istd::CIndex2d(0, 4)));
			i2d::CVector2d translation(res.GetAt(istd::CIndex2d(0, 2)), res.GetAt(istd::CIndex2d(0, 5)));
			i2d::CVector2d perspective(res.GetAt(istd::CIndex2d(0, 6)), res.GetAt(istd::CIndex2d(0, 7)));

			result.SetAffinePart(i2d::CAffine2d(rotationMatrix, translation));
			result.SetPerspAxis(perspective);

			lensCorrFactor = res.GetAt(istd::CIndex2d(0, 8));

			return true;
		}
	}

	if (allowRotation && allowScale && allowAnisotropic && allowTranslation && (degsOfFreedom >= 7)){	// try affine transformation
		imath::CVarMatrix A(istd::CIndex2d(7, degsOfFreedom));
		imath::CVarMatrix x(istd::CIndex2d(1, degsOfFreedom));

		for (int i = 0; i < positionsCount; ++i){
			const i2d::CVector2d& nomPos = nominalPositionsPtr[i];
			const i2d::CVector2d& foundPos = foundPositionsPtr[i];

			i2d::CVector2d foundDiffVector = foundPos - opticalCenter;
			double foundDist = foundDiffVector.GetLength();

			A.SetAt(istd::CIndex2d(0, i * 2 + 0), nomPos.GetX());
			A.SetAt(istd::CIndex2d(1, i * 2 + 0), nomPos.GetY());
			A.SetAt(istd::CIndex2d(2, i * 2 + 0), 1);
			A.SetAt(istd::CIndex2d(3, i * 2 + 0), 0);
			A.SetAt(istd::CIndex2d(4, i * 2 + 0), 0);
			A.SetAt(istd::CIndex2d(5, i * 2 + 0), 0);
			A.SetAt(istd::CIndex2d(6, i * 2 + 0), foundDiffVector.GetX() * foundDist);
			x.SetAt(istd::CIndex2d(0, i * 2 + 0), foundPos.GetX());

			A.SetAt(istd::CIndex2d(0, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(1, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(2, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(3, i * 2 + 1), nomPos.GetX());
			A.SetAt(istd::CIndex2d(4, i * 2 + 1), nomPos.GetY());
			A.SetAt(istd::CIndex2d(5, i * 2 + 1), 1);
			A.SetAt(istd::CIndex2d(6, i * 2 + 1), foundDiffVector.GetY() * foundDist);
			x.SetAt(istd::CIndex2d(0, i * 2 + 1), foundPos.GetY());
		}

		imath::CVarMatrix res(istd::CIndex2d(1, 7));
		if (A.GetSolvedLSP(x, res)){
			i2d::CMatrix2d rotationMatrix(
						res.GetAt(istd::CIndex2d(0, 0)), res.GetAt(istd::CIndex2d(0, 3)),
						res.GetAt(istd::CIndex2d(0, 1)), res.GetAt(istd::CIndex2d(0, 4)));
			i2d::CVector2d translation(res.GetAt(istd::CIndex2d(0, 2)), res.GetAt(istd::CIndex2d(0, 5)));

			result.SetAffinePart(i2d::CAffine2d(rotationMatrix, translation));
			result.SetPerspAxis(i2d::CVector2d::GetZero());

			lensCorrFactor = res.GetAt(istd::CIndex2d(0, 6));

			return true;
		}
	}

	if (allowRotation && allowScale && allowTranslation && (degsOfFreedom >= 5)){	// try affine transformation
		imath::CVarMatrix A(istd::CIndex2d(5, degsOfFreedom));
		imath::CVarMatrix x(istd::CIndex2d(1, degsOfFreedom));

		for (int i = 0; i < positionsCount; ++i){
			const i2d::CVector2d& nomPos = nominalPositionsPtr[i];
			const i2d::CVector2d& foundPos = foundPositionsPtr[i];

			i2d::CVector2d foundDiffVector = foundPos - opticalCenter;
			double foundDist = foundDiffVector.GetLength();

			A.SetAt(istd::CIndex2d(0, i * 2 + 0), nomPos.GetX());
			A.SetAt(istd::CIndex2d(1, i * 2 + 0), 1);
			A.SetAt(istd::CIndex2d(2, i * 2 + 0), -nomPos.GetY());
			A.SetAt(istd::CIndex2d(3, i * 2 + 0), 0);
			A.SetAt(istd::CIndex2d(4, i * 2 + 0), foundDiffVector.GetX() * foundDist);
			x.SetAt(istd::CIndex2d(0, i * 2 + 0), foundPos.GetX());

			A.SetAt(istd::CIndex2d(0, i * 2 + 1), nomPos.GetY());
			A.SetAt(istd::CIndex2d(1, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(2, i * 2 + 1), nomPos.GetX());
			A.SetAt(istd::CIndex2d(3, i * 2 + 1), 1);
			A.SetAt(istd::CIndex2d(4, i * 2 + 1), foundDiffVector.GetY() * foundDist);
			x.SetAt(istd::CIndex2d(0, i * 2 + 1), foundPos.GetY());
		}

		imath::CVarMatrix res(istd::CIndex2d(1, 5));
		if (A.GetSolvedLSP(x, res)){
			i2d::CMatrix2d rotationMatrix(
						res.GetAt(istd::CIndex2d(0, 0)), -res.GetAt(istd::CIndex2d(0, 2)),
						res.GetAt(istd::CIndex2d(0, 2)), res.GetAt(istd::CIndex2d(0, 0)));
			i2d::CVector2d translation(res.GetAt(istd::CIndex2d(0, 1)), res.GetAt(istd::CIndex2d(0, 3)));

			result.SetAffinePart(i2d::CAffine2d(rotationMatrix, translation));
			result.SetPerspAxis(i2d::CVector2d::GetZero());

			lensCorrFactor = res.GetAt(istd::CIndex2d(0, 4));

			return true;
		}
	}

	if (allowScale && allowTranslation && allowAnisotropic && (degsOfFreedom >= 5)){	// try anisotropic scaling transformation
		imath::CVarMatrix A(istd::CIndex2d(5, degsOfFreedom));
		imath::CVarMatrix x(istd::CIndex2d(1, degsOfFreedom));

		for (int i = 0; i < positionsCount; ++i){
			const i2d::CVector2d& nomPos = nominalPositionsPtr[i];
			const i2d::CVector2d& foundPos = foundPositionsPtr[i];

			i2d::CVector2d foundDiffVector = foundPos - opticalCenter;
			double foundDist = foundDiffVector.GetLength();

			A.SetAt(istd::CIndex2d(0, i * 2 + 0), nomPos.GetX());
			A.SetAt(istd::CIndex2d(1, i * 2 + 0), 0);
			A.SetAt(istd::CIndex2d(2, i * 2 + 0), 1);
			A.SetAt(istd::CIndex2d(3, i * 2 + 0), 0);
			A.SetAt(istd::CIndex2d(4, i * 2 + 0), foundDiffVector.GetX() * foundDist);
			x.SetAt(istd::CIndex2d(0, i * 2 + 0), foundPos.GetX());

			A.SetAt(istd::CIndex2d(0, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(1, i * 2 + 1), nomPos.GetY());
			A.SetAt(istd::CIndex2d(2, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(3, i * 2 + 1), 1);
			A.SetAt(istd::CIndex2d(4, i * 2 + 1), foundDiffVector.GetY() * foundDist);
			x.SetAt(istd::CIndex2d(0, i * 2 + 1), foundPos.GetY());
		}

		imath::CVarMatrix res(istd::CIndex2d(1, 5));
		if (A.GetSolvedLSP(x, res)){
			i2d::CMatrix2d rotationMatrix(
						res.GetAt(istd::CIndex2d(0, 0)), 0,
						0, res.GetAt(istd::CIndex2d(0, 1)));
			i2d::CVector2d translation(res.GetAt(istd::CIndex2d(0, 2)), res.GetAt(istd::CIndex2d(0, 3)));

			result.SetAffinePart(i2d::CAffine2d(rotationMatrix, translation));
			result.SetPerspAxis(i2d::CVector2d::GetZero());

			lensCorrFactor = res.GetAt(istd::CIndex2d(0, 4));

			return true;
		}
	}

	if (allowScale && allowTranslation && (degsOfFreedom >= 4)){	// try isotropic scaling transformation
		imath::CVarMatrix A(istd::CIndex2d(4, degsOfFreedom));
		imath::CVarMatrix x(istd::CIndex2d(1, degsOfFreedom));

		for (int i = 0; i < positionsCount; ++i){
			const i2d::CVector2d& nomPos = nominalPositionsPtr[i];
			const i2d::CVector2d& foundPos = foundPositionsPtr[i];

			i2d::CVector2d foundDiffVector = foundPos - opticalCenter;
			double foundDist = foundDiffVector.GetLength();

			A.SetAt(istd::CIndex2d(0, i * 2 + 0), nomPos.GetX());
			A.SetAt(istd::CIndex2d(1, i * 2 + 0), 1);
			A.SetAt(istd::CIndex2d(2, i * 2 + 0), 0);
			A.SetAt(istd::CIndex2d(3, i * 2 + 0), foundDiffVector.GetX() * foundDist);
			x.SetAt(istd::CIndex2d(0, i * 2 + 0), foundPos.GetX());

			A.SetAt(istd::CIndex2d(0, i * 2 + 1), nomPos.GetY());
			A.SetAt(istd::CIndex2d(1, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(2, i * 2 + 1), 1);
			A.SetAt(istd::CIndex2d(3, i * 2 + 1), foundDiffVector.GetY() * foundDist);
			x.SetAt(istd::CIndex2d(0, i * 2 + 1), foundPos.GetY());
		}

		imath::CVarMatrix res(istd::CIndex2d(1, 4));
		if (A.GetSolvedLSP(x, res)){
			i2d::CMatrix2d rotationMatrix(
						res.GetAt(istd::CIndex2d(0, 0)), 0,
						0, res.GetAt(istd::CIndex2d(0, 0)));
			i2d::CVector2d translation(res.GetAt(istd::CIndex2d(0, 1)), res.GetAt(istd::CIndex2d(0, 2)));

			result.SetAffinePart(i2d::CAffine2d(rotationMatrix, translation));
			result.SetPerspAxis(i2d::CVector2d::GetZero());

			lensCorrFactor = res.GetAt(istd::CIndex2d(0, 3));

			return true;
		}
	}

	if (allowTranslation && (degsOfFreedom >= 3)){	// try translation function
		imath::CVarMatrix A(istd::CIndex2d(3, degsOfFreedom));
		imath::CVarMatrix x(istd::CIndex2d(1, degsOfFreedom));

		for (int i = 0; i < positionsCount; ++i){
			const i2d::CVector2d& nomPos = nominalPositionsPtr[i];
			const i2d::CVector2d& foundPos = foundPositionsPtr[i];

			i2d::CVector2d foundDiffVector = foundPos - opticalCenter;
			double foundDist = foundDiffVector.GetLength();

			A.SetAt(istd::CIndex2d(0, i * 2 + 0), 1);
			A.SetAt(istd::CIndex2d(1, i * 2 + 0), 0);
			A.SetAt(istd::CIndex2d(2, i * 2 + 0), foundDiffVector.GetX() * foundDist);
			x.SetAt(istd::CIndex2d(0, i * 2 + 0), foundPos.GetX() - nomPos.GetX());

			A.SetAt(istd::CIndex2d(0, i * 2 + 1), 0);
			A.SetAt(istd::CIndex2d(1, i * 2 + 1), 1);
			A.SetAt(istd::CIndex2d(2, i * 2 + 1), foundDiffVector.GetY() * foundDist);
			x.SetAt(istd::CIndex2d(0, i * 2 + 1), foundPos.GetY() - nomPos.GetY());
		}

		imath::CVarMatrix res(istd::CIndex2d(1, 3));
		if (A.GetSolvedLSP(x, res)){
			i2d::CVector2d translation(res.GetAt(istd::CIndex2d(0, 0)), res.GetAt(istd::CIndex2d(0, 1)));

			result.SetAffinePart(i2d::CAffine2d(i2d::CMatrix2d::GetIdentity(), translation));
			result.SetPerspAxis(i2d::CVector2d::GetZero());

			lensCorrFactor = res.GetAt(istd::CIndex2d(0, 2));

			return true;
		}
	}

	return false;
}


} // namespace iipr


