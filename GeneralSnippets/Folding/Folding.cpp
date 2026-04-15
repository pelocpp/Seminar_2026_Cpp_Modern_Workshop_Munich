// =====================================================================================
// Folding.cpp // Variadic Templates // Folding
// =====================================================================================

module;

#include "../ScopedTimer/ScopedTimer.h"

module modern_cpp:folding;


namespace Folding_Seminar {

    template <typename ... TArgs> // types pack
        // requires
    void transport(TArgs ... args) {  // parameter pack, pack them

        auto list = { args ... }; // unpack

        for (auto n : list) {

            std::println(">>> {}", n);
        }


    }

    void test_transport_01()
    {
        transport( 1, 2, 3, 4, 5 );  // pack them
    }

    class DoSomething
    {
    private:
        int m_var1;
        int m_var2;
        int m_var3;

    public:
        DoSomething() : m_var1{}, m_var2{}, m_var3{} {
            std::cout << "c'tor()" << std::endl;
        }

        DoSomething(int n) : m_var1{ n }, m_var2{}, m_var3{} {
            std::cout << "c'tor(int)" << std::endl;
        }

        DoSomething(int n, int m) : m_var1{ n }, m_var2{ m }, m_var3{} {
            std::cout << "c'tor(int, int)" << std::endl;
        }

        DoSomething(int n, int m, int k) : m_var1{ n }, m_var2{ m }, m_var3{ k } {
            std::cout << "c'tor(int, int, int)" << std::endl;
        }

        friend std::ostream& operator<< (std::ostream&, const DoSomething&);
    };

    std::ostream& operator<< (std::ostream& os, const DoSomething& obj) {
        os
            << "var1: " << obj.m_var1
            << ", var2: " << obj.m_var2
            << ", var3: " << obj.m_var3;

        return os;
    }

    template <typename T, typename ... TArgs>
    std::unique_ptr<T> my_make_unique( TArgs ... args) {  // pack them

        std::unique_ptr<T> ptr{ new T{ args ... } }; // unpack
        return ptr;
    }

    template <typename T, typename ... TArgs>
    std::unique_ptr<T> my_make_unique_perfect(TArgs&& ... args) {  // pack them

        std::unique_ptr<T> ptr{ new T{  std::forward<TArgs>(args) ...  } }; // unpack
        return ptr;
    }

    // Real World use Case
    void test_transport()
    {
        //std::unique_ptr<DoSomething> ptr = 
        //    std::make_unique<DoSomething>(1, 2, 3);

        int n = 1;

      std::unique_ptr<DoSomething> ptr = 
          my_make_unique_perfect<DoSomething>(n, 2, 3);
    }
}

namespace Folding {

    /* folding examples: introduction
    */

    template<typename... TArgs>
    static auto anotherAdder(TArgs... args) {
        return (... + args);  // unary left fold
    }

    static void test_01() {
        int sum = anotherAdder(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        std::cout << "Sum from 1 up to 10: " << sum << std::endl;
    }

    template<typename... TArgs>
    static void printer(TArgs... args) {
        // binary left fold (init == ostream)
        (std::cout << ... << args) << std::endl;
    }

    // demonstrating fold expressions
    static void test_02() {
        printer(1, 2, 3, "ABC", "DEF", "GHI");
    }

    // =================================================================================
    /* demonstrating all four versions of folding expressions
    */

    template<typename... TArgs>
    static auto anotherSubtracterBRF(TArgs... args) {
        return (args - ... - 0);  // binary right fold (init == 0)
    }

    static void test_03a() {
        // binary right fold: 1 - (2 - (3 - (4 - ( 5 - 0)))) = 3
        int result = anotherSubtracterBRF(1, 2, 3, 4, 5);
        std::cout << "BRF: 1 - (2 - (3 - (4 - ( 5 - 0)))): " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterBLF(TArgs... args) {
        return (0 - ... - args);  // binary left fold (init == 0)
    }

    static void test_03b() {
        // binary left fold: ((((0 - 1) - 2) - 3) - 4) - 5 =  -15
        int result = anotherSubtracterBLF(1, 2, 3, 4, 5);
        std::cout << "BLF: ((((0 - 1) - 2) - 3) - 4) - 5: " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterURF(TArgs... args) {
        return (args - ...);  // unary right fold
    }

    static void test_03c() {
        // unary right fold: 1 - (2 - (3 - (4 - 5))) = 3
        int result = anotherSubtracterURF(1, 2, 3, 4, 5);
        std::cout << "URF: 1 - (2 - (3 - (4 - 5))): " << result << std::endl;
    }

    // ----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterULF(TArgs... args) {
        return (... - args);  // unary left fold
    }

    static void test_03d() {
        // unary left fold: ((((1 - 2) - 3) - 4) - 5 = -13
        int result = anotherSubtracterULF(1, 2, 3, 4, 5);
        std::cout << "URF: ((((1 - 2) - 3) - 4) - 5: " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    // Folding over a comma: ',' operator
    // (left or right folding is the same in this case)

    template <typename... TArgs>
    static void printerWithSeperatorRight(TArgs... args) {
        std::string sep = " ";
        ((std::cout << args << sep), ...) << std::endl;
    }

    template <typename... TArgs>
    static void printerWithSeperatorLeft(TArgs... args) {
        std::string sep = " ";
        (... , (std::cout << args << sep)) << std::endl;
    }

    // demonstrating fold expressions
    static void test_04() {
        printerWithSeperatorRight(1, 2, 3, "ABC", "DEF", "GHI");
        printerWithSeperatorLeft (1, 2, 3, "ABC", "DEF", "GHI");
    }

    // -----------------------------------------------------------------------

    // Folding over a comma - pack arguments can be handled by a separate function

    template <typename T>
    static std::ostream& handleArg(T arg) {
        std::cout << arg << "-";
        return std::cout;
    }

    template <typename... TArgs>
    static void printerWithSeperator(TArgs... args) {
        (handleArg(args), ...) << std::endl;
    }

    // demonstrating fold expressions
    static void test_05() {
        printerWithSeperator(1, 2, 3, "ABC", "DEF", "GHI");
    }

    // -----------------------------------------------------------------------

    // Performance Comparison
    static auto addFolding(auto ... values) {
        return (... + values);
    }

    static auto addIterating(auto ... values) {
        
        auto list = { values ...};

        //using ElemType = decltype (list)::value_type;
        //ElemType sum{};

        auto sum{ 0 };
        for (auto elem : list) {
            sum += elem;
        }

        return sum;
    }

#ifdef _DEBUG
    constexpr std::size_t MaxIterations = 10'000'000;     // debug
#else
    constexpr std::size_t MaxIterations = 100'000'000;    // release
#endif

    static void test_06_benchmark_folding() {

        ScopedTimer watch{ };

        for (std::size_t i{}; i != MaxIterations; ++i) {
            auto sum{ addFolding(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
        }
    }

    static void test_06_benchmark_iterating() {

        ScopedTimer watch{ };

        for (std::size_t i{}; i != MaxIterations; ++i) {
            auto sum{ addIterating(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
        }
    }

    static std::size_t test_06_benchmark_folding_02() {

        ScopedTimer watch{ };

        std::size_t total{};

        for (std::size_t i{}; i != MaxIterations; ++i) {
            auto sum{ addFolding(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
            total += sum;
        }

        return total;
    }

    static std::size_t test_06_benchmark_iterating_02() {

        ScopedTimer watch{ };

        std::size_t total{};

        for (std::size_t i{}; i != MaxIterations; ++i) {
            auto sum{ addIterating(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
            total += sum;
        }

        return total;
    }

    // =================================================================================
    /* C++ 20: Variadic Capture / Pack Expansion in Lambda Init-Capture
    */

    template <typename... TArgs>
    auto createDelayedPrinter(TArgs&&... args)
    {
        // Variadic Capture: [...args = std::forward<Args>(args)]
        // This captures each element of the pack into the lambda.
        return [...args = std::forward<TArgs>(args)] () {
            
            // using a fold expression to print the captured pack
            (std::cout << ... << args) << std::endl;
        };
    }

    static void test_07()
    {
        auto printMessage = createDelayedPrinter("Hello - ", "Variadic - ", "Capture!");

        // doing something else ...

        // now invoking the wrapper object, including the parameters
        printMessage();
    }
}

void main_folding()
{
    using namespace Folding;

    using namespace Folding_Seminar;

    test_transport();
    return;

    test_01();
    test_02();
    test_03a();
    test_03b();
    test_03c();
    test_03d();
    test_04();
    test_05();
    
    // Benchmarks: need to switch to nano seconds! 
    // Optimizer in Release mode is very aggressive
    test_06_benchmark_folding();
    test_06_benchmark_iterating();
    std::size_t result1{ test_06_benchmark_folding_02() };
    std::size_t result2{ test_06_benchmark_iterating_02() };
    std::println("Result1: {}", result1);
    std::println("Result2: {}", result2);

    test_07();
}

// =====================================================================================
// End-of-File
// =====================================================================================
