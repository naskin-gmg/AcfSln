// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef i2d_CTubeProjectionsGenerator_included
#define i2d_CTubeProjectionsGenerator_included


// Qt includes
#include <QVector>

// ACF includes
#include <i2d/CTubePolyline.h>


namespace iipr
{


/**
	Helper class for generation of list of projection lines in the given tube region.
*/
class CTubeProjectionsGenerator
{
public:
	typedef QVector<i2d::CLine2d> ProjectionLines;

	/**
		Create list of projections for the given tube region.
		\param tube	Input tube region
		\param projectionsCount Number of projections to be generated
		\param projections	List of generated projection lines.
	*/
	static bool GenerateProjections(const i2d::CTubePolyline& tube, int projectionsCount, ProjectionLines& projections);
};


} // namespace iipr


#endif // !i2d_CTubeProjectionsGenerator_included


