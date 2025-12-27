#include <cassert>

/*
Implement a small “settings snapshot” type and prove (with asserts) the difference between copying values and sharing references.

Requirements:

Single file main.cpp.

Define:
struct Settings {
  int volume;       // 0..100
  int brightness;   // 0..100
};

struct ConfigValue {
  Settings s;
};

struct ConfigRef {
  const Settings* s;   // non-owning
};


Implement:
ConfigValue make_value(Settings s);          // stores by value
ConfigRef   make_ref(const Settings& s);     // stores pointer to external


In main():

Create:

Settings base{50, 50};

Create both:
auto cv = make_value(base);
auto cr = make_ref(base);

Mutate base:

base.volume = 10;
base.brightness = 90;


Use assert to prove:

cv.s did not change (value snapshot)
*cr.s did change (reference view)

Add one more test demonstrating dangling danger:

Create a ConfigRef from a temporary scope:

ConfigRef bad;
{
  Settings temp{1,2};
  bad = make_ref(temp);
}


Do not dereference bad.s (UB).

Instead, assert(bad.s != nullptr); and add a comment that it’s dangling.

Then show the safe alternative using ConfigValue in the same scenario, proving it remains valid.

Constraints

C++23 only
No frameworks
No extra logging

*/

struct Settings {
  int volume;       // 0..100
  int brightness;   // 0..100
};

struct ConfigValue {
  Settings s;
};

struct ConfigRef {
  const Settings* s;   // non-owning
};

ConfigValue make_value(Settings s);          // stores by value
ConfigRef   make_ref(const Settings& s);     // stores pointer to external

ConfigValue make_value(Settings s) {
    return ConfigValue{s};
}

ConfigRef make_ref(const Settings& s) {
    return ConfigRef{&s};
}

int main()
{

    Settings base{50, 50};

    // Create both:
    auto cv = make_value(base);
    auto cr = make_ref(base);

    // Mutate base:
    base.volume = 10;
    base.brightness = 90;

    //Use assert to prove:

    //cv.s did not change (value snapshot)
    assert(cv.s.volume == 50);

    //*cr.s did change (reference view)
    assert(cr.s->volume == 10);

    // Add one more test demonstrating dangling danger:
    // Create a ConfigRef from a temporary scope:

    ConfigRef bad;
    {
        Settings temp{1,2};
        bad = make_ref(temp);
    }

    //Do not dereference bad.s (UB).
    // Instead, assert(bad.s != nullptr); and add a comment that it’s dangling.
    assert(bad.s != nullptr); // dangling pointer

    // Then show the safe alternative using ConfigValue in the same scenario, proving it remains valid.   
    ConfigValue good;
    {
        Settings temp{1,2};
        good = make_value(temp);
    }
    assert(good.s.volume == 1); // valid copy

    return 0;
}