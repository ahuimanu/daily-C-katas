#include <cassert>
#include <optional>

/*
Implement a small explicit state machine for a flight leg lifecycle.

Single file main.cpp.

Define:
enum class State { parked, taxi_out, takeoff, cruise, approach, landed, taxi_in };
enum class Event { start_taxi, rotate, climb, begin_approach, touchdown, exit_runway, park };


Implement:
std::optional<State> transition(State s, Event e);

Returns next state if the transition is valid, otherwise std::nullopt.
Valid transitions only:
parked + start_taxi → taxi_out
taxi_out + rotate → takeoff
takeoff + climb → cruise
cruise + begin_approach → approach
approach + touchdown → landed
landed + exit_runway → taxi_in
taxi_in + park → parked
*/

enum class State { parked, taxi_out, takeoff, cruise, approach, landed, taxi_in };
enum class Event { start_taxi, rotate, climb, begin_approach, touchdown, exit_runway, park };

std::optional<State> transition(State s, Event e);

std::optional<State> transition(State s, Event e)
{
    switch (s)
    {
    case State::parked:
        if (e == Event::start_taxi)
            return State::taxi_out;
        break;
    case State::taxi_out:
        if (e == Event::rotate)
            return State::takeoff;
        break;
    case State::takeoff:
        if (e == Event::climb)
            return State::cruise;
        break;
    case State::cruise:
        if (e == Event::begin_approach)
            return State::approach;
        break;
    case State::approach:
        if (e == Event::touchdown)
            return State::landed;
        break;
    case State::landed:
        if (e == Event::exit_runway)
            return State::taxi_in;
        break;
    case State::taxi_in:
        if (e == Event::park)
            return State::parked;
        break;
    default:
        break;
    }
    return std::nullopt;
}


int main()
{

    // Test valid transition
    assert(transition(State::parked, Event::start_taxi) == State::taxi_out);
    assert(transition(State::taxi_out, Event::rotate) == State::takeoff);
    assert(transition(State::takeoff, Event::climb) == State::cruise);
    assert(transition(State::cruise, Event::begin_approach) == State::approach);
    assert(transition(State::approach, Event::touchdown) == State::landed);
    assert(transition(State::landed, Event::exit_runway) == State::taxi_in);
    assert(transition(State::taxi_in, Event::park) == State::parked);

    // Test invalid transitions
    assert(transition(State::parked, Event::touchdown) == std::nullopt);
    assert(transition(State::cruise, Event::rotate) == std::nullopt);
    assert(transition(State::taxi_in, Event::begin_approach) == std::nullopt);

    return 0;
}