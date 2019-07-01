#pragma once

#include "Manager.h"
#include "MainWindow.h"

struct SearchInfo
{
	enum class Type
	{
		None,
		MaxBasepriceMinBuildarea,
		OnStreet,
		MinApartments,
		MinApartmentSameBuildArea,
		MinApartmentSameRooms,
	};
	Type type = Type::None;
	int max_baseprice = 0, min_buildarea = 0, min_apartments = 0, same_buildarea = 0, same_rooms = 0;
	string street;
	bool nvilla = true, svilla = true, apartment = true, building = true;
};

class SearchResidenceWindow : public MainWindow
{
	Q_OBJECT

public:
	SearchResidenceWindow(SearchInfo &info, QWidget *parent = Q_NULLPTR);
	~SearchResidenceWindow();

public slots:
	void cancel_clicked();
	void ok_clicked();
	void type_changed();

private:
	SearchInfo &info;
	SearchInfo::Type type;

	QSpinBox *max_basepriceEdit, *min_buildareaEdit, *min_apartmentsEdit, *same_buildareaEdit, *same_roomsEdit;
	QLineEdit *streetEdit;
	QCheckBox *nvillaBox, *svillaBox, *apartmentBox, *buildingBox;
};
