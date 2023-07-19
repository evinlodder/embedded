#ifndef REG_38e4a51f_e09a_424c_9808_f6c420a4ce63
#define REG_38e4a51f_e09a_424c_9808_f6c420a4ce63

#ifndef __cplusplus
#error "<reg> is a C++ header!"
#endif

#include <cstdint>
#include <bit>

//<reg> defines constants for hardware registers- will be added to as needed
namespace reg {
//function to access data from register
constexpr inline std::uint32_t& data(std::uint32_t reg) {
    return *std::bit_cast<std::uint32_t*>(reg);
}

#if defined(TARGET_IS_TM4C123_RB1)
//*********************************************
// SYSCTL registers
//*********************************************
constexpr std::uint32_t sysctl_rcc2_r = 0x400FE070;
constexpr std::uint32_t sysctl_rcc_r  = 0x400FE060;
constexpr std::uint32_t sysctl_ris    = 0x400FE050;
//*********************************************
// SysTick registers
//*********************************************
constexpr std::uint32_t systick_ctrl_r    = 0xE000E010;
constexpr std::uint32_t systick_reload_r  = 0xE000E014;
constexpr std::uint32_t systick_current_r = 0xE000E018;

#endif
}

#endif
