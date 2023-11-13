#include "general.h"

QString Ui::TCollection_ExtendedStringTochars(TCollection_ExtendedString extString) {
    // 获取 TCollection_ExtendedString 的长度
    Standard_Integer extStringLength = extString.Length();
    // 分配内存并将 TCollection_ExtendedString 复制到 C 语言风格的字符串
    char* cString = new char[extStringLength + 1]; // +1 用于终止空字符 '\0'
    for (Standard_Integer i = 1; i <= extStringLength; ++i) {
        cString[i - 1] = extString.Value(i);
    }
    cString[extStringLength] = '\0'; // 添加终止空字符
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