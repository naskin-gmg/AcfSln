#ifndef iqtcam_CMultiBitmapViewComp_included
#define iqtcam_CMultiBitmapViewComp_included


// Qt includes
#include <QtGui/QGroupBox>
#include <QtGui/QBoxLayout>
#include <QtGui/QLabel>

// ACF includes
#include <iqtgui/TGuiComponentBase.h>
#include <imod/CMultiModelDispatcherBase.h>
#include <istd/IInformationProvider.h>
#include <iqt2d/IViewExtender.h>
#include <iqt2d/IViewProvider.h>
#include <iview/CConsoleGui.h>
#include <iqt/CBitmap.h>


namespace iqtcam
{


class CMultiBitmapViewComp: 
			public iqtgui::TGuiComponentBase<QWidget>,
			public imod::CMultiModelDispatcherBase
{
public:
	typedef iqtgui::TGuiComponentBase<QWidget> BaseClass;

	I_BEGIN_COMPONENT(CMultiBitmapViewComp);
		I_ASSIGN(m_horizontalViewsAttrPtr, "HorizontalViewsCount", "Number of horizontal views", false, 1);
		I_ASSIGN(m_verticalViewsAttrPtr, "VerticalViewsCount", "Number of vertical views", false, 1);
		I_ASSIGN_MULTI_0(m_informationProvidersCompPtr, "InformationProviders", "Information providers", true);
		I_ASSIGN_TO(m_informationModelsCompPtr, m_informationProvidersCompPtr, true);
		I_ASSIGN_MULTI_0(m_viewExtendersCompPtr, "ViewExtenders", "View extenders", true);
	I_END_COMPONENT;

protected:
	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected:
	class CSingleView: public QGroupBox, public iqt2d::IViewProvider
	{
	public:
		typedef QGroupBox BaseClass;

		CSingleView(QWidget* parentPtr, int id = -1, const QString& title = "");

		// called once at initialization stage, at this moment the model should be connected already.
		virtual void Init();
		virtual void SetInspectionResult(int result);

		// reimplemented (iqt2d::IViewProvider)
		virtual int GetViewId() const;
		virtual iview::IShapeView* GetView() const;

	protected:
		// called from Init() to add status labels etc. to the main widget layout (layoutPtr).
		virtual void AddStatusItems(QBoxLayout* layoutPtr);

	protected:
		iview::CImageShape m_backgroundShape;
		imod::TModelWrap<iqt::CBitmap> m_backgroundModel;

		mutable iview::CConsoleGui m_console;
		QLabel* m_statusLabel;
		int m_id;
	};

	virtual CSingleView* CreateView(QWidget* parentPtr, int id = -1, const QString& title = "");

private:
	I_ATTR(int, m_horizontalViewsAttrPtr);
	I_ATTR(int, m_verticalViewsAttrPtr);
	I_MULTIREF(istd::IInformationProvider, m_informationProvidersCompPtr);
	I_MULTIREF(imod::IModel, m_informationModelsCompPtr);
	I_MULTIREF(iqt2d::IViewExtender, m_viewExtendersCompPtr);

	int m_rowCount;
	int m_columnCount;
	int m_viewCount;
	QList<CSingleView*> m_views;
};


} // namespace iqtcam


#endif // !iqtcam_CMultiBitmapViewComp_included
