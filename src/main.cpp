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

int main()
{

    return 0;
}