#ifndef FSM_dad1519b_c659_42ac_81f2_39484c452036
#define FSM_dad1519b_c659_42ac_81f2_39484c452036

//<fsm> defines finite state machines for general use
#include <tick>
#include <initializer_list>
#include <array>

namespace vin {

//moore_fsm provides an interface for a moore machine
//template params:
// -OutputType: output type- usually integers
// -InitialState: initial state of th machine
// -StateCount: number of states
// -InputCount: number of inputs
template<
    typename OutputType,
    std::size_t InitialState,
    std::size_t StateCount,
    std::size_t InputCount
>
class moore_fsm {
public:
    //underlying state_type structure- this is essentially the engine behind the fsm
    struct state_type {
        OutputType out; //output of this state type
        std::uint32_t delay; //delay at this state
        std::array<std::size_t, InputCount * InputCount - 1> next; //array of states to work with
    };
private:
    std::array<state_type, StateCount> m_states;
    state_type m_current_state;
public:
    constexpr moore_fsm() = delete; //no default ctor

    //array constructor initializes m_states to the values in the list
    // note- since there are no exceptions, bound checking won't return anything
    // this will be a likely culprit if there are seg faults.
    constexpr moore_fsm(std::array<state_type, StateCount> states) : m_states(states),
                                                                     m_current_state(m_states[InitialState]){}      //returns the output of the current state or zero if initialization failed
    constexpr inline OutputType output() const {
        return m_current_state.out;
    }
    //wait waits the current state's ms delay
    inline void wait() const {
        if(!vin::tick_enabled) {
            vin::tick_init();
        }
        vin::sleep(m_current_state.delay);
    }
    //next progresses the the fsm to the next state given an input
    // -since exceptions are not present, we will return 0 on success and 1 on out-of-bounds
    [[maybe_unused]] inline uint8_t next(std::size_t input) {
        if(input > ((2 << InputCount) - 1)) {
            return 1;
        }
        m_current_state = m_states[m_current_state.next[input]];
        return 0;
    }
};

}

#endif
