
#include <array>
#include <span>
#include <cassert>
#include <functional>
#include <utility>

/*
Task

Implement a tiny Resource Acquisition Is Initialization (RAII) scope-exit guard (defer) that runs a callable exactly once when the scope ends.

Requirements
Single file main.cpp.

Implement:

class Defer {
public:
  template<class F>
  explicit Defer(F&& f);

  ~Defer();

  Defer(const Defer&) = delete;
  Defer& operator=(const Defer&) = delete;

  Defer(Defer&& other) noexcept;
  Defer& operator=(Defer&& other) noexcept;

  void dismiss() noexcept;   // prevents execution
};


Rules:

Runs the stored callable in the destructor if not dismissed.
Move transfers responsibility; moved-from does nothing.
Callable must be invoked at most once.
No dynamic allocation in your code (standard library may allocate internally if you choose to use type-erasure, but aim to avoid it).

In main() use assert to verify:

Basic execution on scope exit:
A counter increments once.

dismiss() prevents execution.

Move behavior:

Moving a Defer into an inner scope results in exactly one execution total (not zero, not two).

Constraints

C++23
No frameworks
No logging
Use assert only

*/

class Defer {
public:
    template<class F>
    explicit Defer(F&& f);

    ~Defer() noexcept;

    Defer(const Defer&) = delete;
    Defer& operator=(const Defer&) = delete;
    
    Defer(Defer&& other) noexcept;
    Defer& operator=(Defer&& other) noexcept;

    void dismiss() noexcept;   // prevents execution

private:
    std::move_only_function<void()> fn_{};
    bool active_{false};
};

template<class F>
Defer::Defer(F&& f)
    : fn_(std::forward<F>(f))
    , active_(true)
{
}

Defer::~Defer() noexcept
{
    if (active_ && fn_) {
        fn_();
    }
}

Defer::Defer(Defer&& other) noexcept
    : fn_(std::move(other.fn_))
    , active_(other.active_)
{
    other.active_ = false;
}

Defer& Defer::operator=(Defer&& other) noexcept
{
    if (this == &other) {
        return *this;
    }

    if (active_ && fn_) {
        fn_();
    }

    fn_ = std::move(other.fn_);
    active_ = other.active_;
    other.active_ = false;

    return *this;
}

void Defer::dismiss() noexcept
{
    active_ = false;
}

int main()
{
    // Basic execution on scope exit
    int counter = 0;
    {
        Defer d([&] { ++counter; });
    }
    assert(counter == 1);

    // dismiss() prevents execution
    counter = 0;
    {
        Defer d([&] { ++counter; });
        d.dismiss();
    }
    assert(counter == 0);

    // Move behavior: exactly one execution total
    counter = 0;
    {
        Defer outer([&] { ++counter; });
        {
            Defer inner(std::move(outer));
        }
    }
    assert(counter == 1);

    return 0;
}