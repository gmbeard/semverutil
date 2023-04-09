#ifndef RGBCTL_TESTING_HPP_INCLUDED
#define RGBCTL_TESTING_HPP_INCLUDED

#include <initializer_list>
#include <stdexcept>
#include <tuple>

#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)
#define TEST(fn) std::make_pair(STRINGIFY(fn), fn)
#define EXPECT(cond)                                                           \
    do {                                                                       \
        if (!(cond))                                                           \
            throw ::semver::testing::TestFailure(                              \
                "Expectation not met: " STRINGIFY(cond) "\n  in " __FILE__ ":" STRINGIFY(__LINE__));         \
    }                                                                          \
    while (0)
#define IGNORE(fn, reason)                                                     \
    std::make_pair(                                                            \
        STRINGIFY(fn), []() { throw ::semver::testing::TestIgnored(reason); })

#define TEST_WITH_CONTEXT(fn)                                                  \
    auto fn([[maybe_unused]] ::semver::testing::TestContext const& test_context)

namespace semver::testing
{

struct TestContext
{
    int argc;
    char const** argv;
};

using TestFunction = auto (*)() -> void;
using TestFunctionWithContext = auto (*)(TestContext const&) -> void;
using Test = std::pair<char const*, TestFunction>;
using TestWithContext = std::pair<char const*, TestFunctionWithContext>;

struct TestFailure : std::logic_error
{
    TestFailure(char const* msg);
};

struct TestIgnored : std::logic_error
{
    TestIgnored(char const* msg);
};

auto run_with_context(int, char const**, std::initializer_list<TestWithContext>)
    -> int;
auto run(std::initializer_list<Test>) -> int;

} // namespace semver::testing

#endif // RGBCTL_TESTING_HPP_INCLUDED
