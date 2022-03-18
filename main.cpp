#include <iostream>
#include <vector>

// Школа разработчиков 17.03.2022

using namespace std;

// Пример цикла

template <int N>
struct Fibonacci {
    static constexpr int val = Fibonacci<N - 1>::val + Fibonacci<N - 2>::val;
};

template <> // это называется специализация шаблона Fibonacci
struct Fibonacci<0> {
    static constexpr int val = 1;
};

template <> // это тоже специализация
struct Fibonacci<1> {
    static constexpr int val = 1;
};

template <typename T, T t>
struct IntegralConstant {
    using Type = T;
    static constexpr T value = t;
};

using TrueType = IntegralConstant<bool, true>;
using FalseType = IntegralConstant<bool, false>;

template <bool value, typename TrueType, typename FalseType>
struct IsConditional {
    using Type = FalseType;
};

template <typename TrueType, typename FalseType>
struct IsConditional<false, TrueType, FalseType> {
    using Type = TrueType;
};

// terminal element
struct NullStruct {
    ~NullStruct() = delete;
};

template <typename FirstType, typename SecondType = NullStruct>
struct ListOfTypes {
    using Head = FirstType;
    using Tail = SecondType;
};

template <typename T>
void print_list_of_types()
{
    cout << "type is " << typeid(typename T::Head).name() << endl;
    print_list_of_types<typename T::Tail>();
}

template <>
void print_list_of_types<NullStruct>()
{
    cout << "type is NullStruct" << endl;
}

// это из STL TypeTrades
template <typename T1, typename T2>
struct IsSame : FalseType {
};

template <typename T>
struct IsSame<T, T> : TrueType {
};

// напишем структуру, проверяющую, есть ли Type в ListOfTypes
template <typename CheckableType, typename CheckType>
struct AnyOf {
    static constexpr bool value = IsConditional<
        IsSame<
            CheckableType, typename CheckType::Head>::value,
        TrueType,
        AnyOf<
            CheckableType, typename CheckType::Tail>>::Type::value;
};

template <typename CheckableType>
struct AnyOf<CheckableType, NullStruct> {
    static constexpr bool value = FalseType::value;
};

// talking about sfinae now
// let's overload templates by params!
template <typename T>
// operator , runs all expressions (first three expressions are instancing, so here we check if
// std::begin(a) can be instanced, then we return object void() and feed it to decltype)
auto print(T a) -> decltype(begin(T()), end(T()), cout << typename T::value_type(), void())
{
    for (const auto& val : a) {
        cout << val << " ";
    }
    cout << endl;
}

template <typename T>
auto print(T a) -> decltype(cout << T(), void())
{
    cout << a << endl;
}

// sfinae
// We described two functions print. 'auto func -> decltype' is just checking if template parameter
// meets ths requirements, but if it's not compiler DOES NOT RAISE COMPILE ERROR. So we use it.
// It is overload if we use 100% of brain (meme).

int main()
{
    cout << "fibonacci(11) = " << Fibonacci<11>::val << endl;
    // HELL YEAH FIBONACCI ALGO AT COMPILT TIME!!!!! WHYYY!!  I DONT KNOW!!!!!!!!!!!!!

    IsConditional<true, double, int>::Type i; // type is double
    IsConditional<false, double, int>::Type j; // type is int
    // so it's a condition!!! METACONDITION!!!!!!!!

    // at this point we have meta-loop and meta-condition. for real, why?
    // naaah better receive another pile of template shit. IN YOUR FACE!!
    ListOfTypes<double, ListOfTypes<int, ListOfTypes<char>>> val;

    // typeid(...).type() aaand??
    print_list_of_types<ListOfTypes<double, ListOfTypes<int, ListOfTypes<char>>>>();

    // it should be zero... it isn't...... we live in society........
    cout << "is int among char & double (amogus...) : "
         << AnyOf<int, ListOfTypes<char, ListOfTypes<double>>>::value << endl;

    print(4);
    vector<int> v = {1, 2, 3};
    print(v);

    // decltype deduces type!
    // int& foo() {}
    // decltype(foo()) = int&

    return 0;
}