#ifndef GALUAFIELD_H
#define GALUAFIELD_H
#include <vector>
//небольшой синлтон, который нужен затем, чтобы хранить обратные элементы для некого поля GF(p). существует для ускорения, так как
//пользоваться числами в поле галуа я буду все равно только при факторизации полиномов, и в течении долгого времени все будет происходить в одном поле
//числа GaluaFieldNumber берут p из синлтона GaluaField для удобства их создания.
class GaluaField
{
    long long int _p;
    //индекс - элемент a, а значение вектора - a^-1. нулевой элемент не имеет обратного
    std::vector<int> reverse_elements;
    void countReverseElements();
    bool over_integer = false;
    bool lock_changing_reverse = false;
public:
    static GaluaField& get();
    static void initialize(int p_value);

    static int reverse(int element);
    static long long int p();
    static bool isOverIntegers();
    static void setLockChangingReverseElements(bool enabled);
private:
    GaluaField();
    GaluaField(const GaluaField&) = delete;
    void operator=(const GaluaField&) = delete;
    friend class GfNumber;

};
//получает значение p от GaluaField только в момент инициализации от long long int, иначе от других GfNumber.
//в арифметических операциях могут учавствовать числа только с одинаковым p
class GfNumber
{
    long long int value;
public:
    GfNumber(long long int value = 0);
    GfNumber(const GfNumber & cop);
    GfNumber& operator=(long long int value);
    GfNumber& operator=(const GfNumber & cop);
    long long int p() const;

    GfNumber& operator+=(const GfNumber & right);
    GfNumber& operator-=(const GfNumber & right);
    GfNumber& operator*=(const GfNumber & right);
    GfNumber& operator/=(const GfNumber & right);

    bool operator==(const GfNumber & right) const;
    bool operator!=(const GfNumber & right) const;

    friend GfNumber operator+(const GfNumber & left, const GfNumber & right);
    friend GfNumber operator-(const GfNumber & left, const GfNumber & right);
    friend GfNumber operator*(const GfNumber & left, const GfNumber & right);
    friend GfNumber operator/(const GfNumber & left, const GfNumber & right);
    long long int toInt() const;

};
GfNumber operator+(const GfNumber & left, const GfNumber & right);
GfNumber operator-(const GfNumber & left, const GfNumber & right);
GfNumber operator*(const GfNumber & left, const GfNumber & right);
GfNumber operator/(const GfNumber & left, const GfNumber & right);
GfNumber operator/(int left, const GfNumber & right);
GfNumber operator/(const GfNumber & left, int right);
#endif // GALUAFIELD_H
