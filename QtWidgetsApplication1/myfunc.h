#pragma once
#include <QString>
#include"occ.h"


QString TCollection_ExtendedStringTochars(TCollection_ExtendedString extString);
QString GetLabelName(TDF_Label& Label00);

class STEPTree {
public:
	QString name;
	std::vector<QString> childName;
};