#include <type_traits>

namespace TypeListUtilities {

    template <typename... Types>
    struct TypeList {};

    namespace details {
        template <typename T, typename U>
        struct IsTypeInList;

        template <typename T, typename... Rest>
        struct IsTypeInList<T, TypeList<T, Rest...>> : std::true_type {};

        template <typename T, typename First, typename... Rest>
        struct IsTypeInList<T, TypeList<First, Rest...>> : IsTypeInList<T, TypeList<Rest...>> {};

        template <typename T>
        struct IsTypeInList<T, TypeList<>> : std::false_type {};

        template <typename T, typename U, std::size_t N>
        struct TypeIndexHelper;

        template <typename T, typename... Rest, std::size_t N>
        struct TypeIndexHelper<T, TypeList<T, Rest...>, N> : std::integral_constant<std::size_t, N> {};

        template <typename T, typename First, typename... Rest, std::size_t N>
        struct TypeIndexHelper<T, TypeList<First, Rest...>, N> : TypeIndexHelper<T, TypeList<Rest...>, N+1> {};

        template <typename T, std::size_t N>
        struct TypeIndexHelper<T, TypeList<>, N>:std::integral_constant<std::size_t, N+1> {};
    }


    template <std::size_t Index, typename List>
    struct TypeAt;

    template <std::size_t Index, typename First, typename... Rest>
    struct TypeAt<Index, TypeList<First, Rest...>> {
        using Type = typename TypeAt<Index-1, TypeList<Rest...>>::Type;
    };

    template <typename First, typename... Rest>
    struct TypeAt<0, TypeList<First, Rest...>> {
        using Type = First;
    };

    template <typename List>
    struct ListSize;

    template <typename... Types>
    struct ListSize<TypeList<Types...>>
            : std::integral_constant<std::size_t, sizeof...(Types)> {};


    template <typename T, typename List>
    struct Contains : details::IsTypeInList<T, List>::type {};

    template <typename T, typename List>
    struct IndexOf : details::TypeIndexHelper<T, List, 0> {};

    template <typename NewType, typename List>
    struct Prepend;

    template <typename NewType, typename... Types>
    struct Prepend<NewType, TypeList<Types...>> {
        using Result = TypeList<NewType, Types...>;
    };


    template <typename NewType, typename List>
    struct Append;

    template <typename NewType, typename... Types>
    struct Append<NewType, TypeList<Types...>> {
        using Result = TypeList<Types..., NewType>;
    };

}

using namespace TypeListUtilities;

using EmptyList = TypeList<>;
using IntList = TypeList<int>;
using TestList = TypeList<char, double, int, float>;

static_assert(ListSize<EmptyList>::value == 0);
static_assert(ListSize<TestList>::value == 4);

static_assert(Contains<int, TestList>::value);
static_assert(!Contains<long, TestList>::value);

static_assert(IndexOf<double, TestList>::value == 1);
static_assert(IndexOf<float, TestList>::value == 3);

using PrependedList = Prepend<bool, TestList>::Result;
static_assert(std::is_same_v<TypeAt<0, PrependedList>::Type, bool>);
static_assert(ListSize<PrependedList>::value == 5);

using AppendedList = Append<long, TestList>::Result;
static_assert(std::is_same_v<TypeAt<4, AppendedList>::Type, long>);

static_assert(std::is_same_v<TypeAt<2, TestList>::Type, int>);

