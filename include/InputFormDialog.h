#ifndef INPUTFORMDIALOG_H_INCLUDED
#define INPUTFORMDIALOG_H_INCLUDED

#include <iterator>
#include <vector>
#include <utility>

#include <QAbstractSpinBox>
#include <QtGlobal>
#include <QString>
#include <QVariant>

namespace InputFormDialog
{

class FormData
{
public:
    QVariant& operator[](const QString& key);
    
    std::vector<std::pair<QString, QVariant>>::iterator begin();
    std::vector<std::pair<QString, QVariant>>::iterator end();
    
    template <typename T>
    T at(const QString& key) const
    {
        auto value = getValue(key);
        
        Q_ASSERT_X(!value.isNull(), "FormTemplate::at", "invalid key");
        Q_ASSERT_X(value.canConvert<T>(), "FormTemplate::at", "invalid type");
        
        return value.value<T>();
    }
    
private:
    QVariant getValue(const QString& key) const;
    
    std::vector<std::pair<QString, QVariant>> data_;
};

struct FormOptions
{
    bool listReturnsIndex = false;
    bool listDisplaysAsRadios = false;
    
    int numericMin =       0;
    int numericMax = INT_MAX;
    int numericPrecision = 2;
};

bool getInput(const QString& title, FormData& data, const FormOptions& options = FormOptions());

}

#endif // INPUTFORMDIALOG_H_INCLUDED