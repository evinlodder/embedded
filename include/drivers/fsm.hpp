#ifndef FSM_dad1519b_c659_42ac_81f2_39484c452036
#define FSM_dad1519b_c659_42ac_81f2_39484c452036

//<fsm> defines finite state machines for general use

namespace vin {

//moore_fsm provides an interface for a moore machine
//template params:
// -State: enumerable for state types
// -OutputType: output type- usually integers
// -InitialState: initial state of th machine
// -StateCount: number of states
// -InputCount: number of inputs
template<
    typename State,
    typename OutputType,
    State InitialState,
    std::size_t StateCount,
    std::size_t InputCount
>
class moore_fsm {
public:
    //underlying state_type structure- this is essentially the engine behind the fsm
    struct state_type {
        OutputType out; //output of this state type
        std::uint32_t delay; //delay at this state
        std::size_t next[InputCount * InputCount]; //array of states to work with
    };
private:
    state_type m_states[StateCount];
    State m_current_state = InitialState;
public:
    constexpr moore_fsm() = delete; //no default ctor
    constexpr moore_fsm(state_type states[StateCount]) : m_states(states) {} //constructor with machine
    
}

}

#endif
