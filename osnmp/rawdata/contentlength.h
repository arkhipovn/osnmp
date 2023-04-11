#ifndef LENGTH_H
#define LENGTH_H

#include <QDataStream>
#include "decoderesult.h"

namespace osnmp {
    class ContentLength
    {
    public:
        enum Form : quint8 {
            DefiniteForm,       // ShortForm или LongForm
            // В этом случае в конце блока закодированных данных должны стоять октеты 00 00, явно указывающие на его завершение.
            // Кодирование с неопределённой длиной разрешено только для конструктивных типов данных
            IndefiniteForm      // длина не известна
        };

        constexpr ContentLength(Form form) : form_(form), length_(0) {}
        constexpr ContentLength(size_t size = 0, Form form = Form::DefiniteForm) : form_(form), length_(size) {}
        constexpr ContentLength(const ContentLength &len) : form_(len.form_), length_(len.length_) {}

        constexpr operator size_t() const { return length_; }
        constexpr bool operator ==(size_t s) const { return length_ == s; }
        constexpr bool operator !=(size_t s) const { return length_ != s; }

        ContentLength &operator =(const ContentLength &len);

        constexpr bool isIndefiniteForm() const { return form_ == Form::IndefiniteForm; }
        constexpr size_t size() const { return length_; }
        constexpr size_t length() const { return length_; }
        constexpr Form form() const { return form_; }

        void setSize(size_t size) { length_ = size; }
        void setForm(Form form) { form_ = form; }

        ParseResult decode(QDataStream &stream);
        ParseError encode(QDataStream &stream) const;

    private:
        Form form_;
        size_t length_;
    };
}

#endif // LENGTH_H


