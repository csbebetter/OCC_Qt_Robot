#include "general.h"

QString Ui::TCollection_ExtendedStringTochars(TCollection_ExtendedString extString) {
    // ��ȡ TCollection_ExtendedString �ĳ���
    Standard_Integer extStringLength = extString.Length();
    // �����ڴ沢�� TCollection_ExtendedString ���Ƶ� C ���Է����ַ���
    char* cString = new char[extStringLength + 1]; // +1 ������ֹ���ַ� '\0'
    for (Standard_Integer i = 1; i <= extStringLength; ++i) {
        cString[i - 1] = extString.Value(i);
    }
    cString[extStringLength] = '\0'; // �����ֹ���ַ�
    QString qString = QString(cString);
    delete[] cString;
    return qString;
}

QString Ui::GetLabelName(TDF_Label& Label00) {
    Handle_TDataStd_Name attrName;
    Label00.FindAttribute(TDataStd_Name::GetID(), attrName);
    TCollection_ExtendedString extString = attrName->Get();
    QString qString = TCollection_ExtendedStringTochars(extString);
    return qString;
}