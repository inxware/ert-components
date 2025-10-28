# EN60730 Specifications for MCU and Software Self-Checks

EN60730 is an international standard for automatic electronic controls used in household and similar applications. For microcontroller units (MCUs) and embedded software, EN60730 defines requirements for self-check mechanisms to ensure functional safety and fault detection. Annex H of this specification defines the requirements for software.

## inxware Implementation Notes
Most of the diagnositcs required to support EN60730 are potentially derived from the tests identified in the ../common directory as EN60730 does not require substnaitally specialised test cases. However some target-specific libraries or for EN60730 may be provided in MCU SDKs and integrated in this directory instead of more generic options.

## Diagnostic Tests and Timing Requirements

### CPU Tests (Class B)
- **CPU Register Test:** Verify all CPU registers at startup and periodically during runtime
- **Program Counter Test:** Check PC functionality at startup
- **Instruction Set Test:** Verify core instructions at startup

### Memory Tests
- **RAM Test (March Test):** Execute at startup and periodically during runtime
- **ROM/Flash Test (CRC):** Verify program memory at startup and periodically
- **Invariable Memory Test:** Check constants and fixed parameters at startup
- **Variable Memory Test:** Verify data integrity continuously during runtime

### Clock Tests
- **Clock Frequency Test:** Monitor clock stability every 100ms
- **Clock Synchronization:** Verify multi-clock systems at startup and periodically

### I/O Tests
- **I/O Register Test:** Check I/O registers at startup
- **I/O Peripheral Test:** Verify I/O functionality periodically during operation

### Program Flow
- **Control Flow Test:** Continuously monitor program execution sequence
- **Runtime Checks:** Implement watchdog and timing supervision continuously

## Application

All tests must be documented and validated according to EN60730 standards.
Some MCU SDKs provide EN60730 libraries to carry out MCU-speicific and optimised tests, however the base-code is typically quite generic, requiring paramterisation with register ranges, types and memory/flash configuration.

# Detailed Requirements

EN 60730 is the European standard for "Automatic electrical controls for household and similar use," with particular focus on safety-related software in appliances. Here's a comprehensive breakdown:

## Standard Structure

**EN 60730-1**: General requirements (main standard)
**EN 60730-2-x**: Particular requirements for specific control types (thermostats, motor controls, etc.)
**Annex H**: Software requirements (the critical section for embedded systems)

## Classification System

EN 60730 classifies controls into three classes based on safety criticality:

**Class A**: Non-safety controls (minimal requirements)
**Class B**: Controls preventing unsafe operation (moderate requirements)
**Class C**: Controls preventing special hazards (most stringent requirements, similar to SIL 2-3)

Most safety-related appliance controls fall under **Class B or C**.

## Annex H - Software Requirements

Annex H is the heart of EN 60730 for embedded software developers. It mandates specific self-test mechanisms:

### H.2 - Program Sequence Test

**Purpose**: Verify the program executes in the correct sequence and detects runaway code.

**Requirements**:
- Must detect interruption of intended program sequence
- Test frequency: Depends on risk assessment but typically every program cycle or periodically

**Implementation methods**:
- **Temporal monitoring**: Checksums calculated at specific points in code
- **Logical monitoring**: Sequence counters that increment in specific order
- **Address monitoring**: Verification that program counter is in valid ranges
- **Program flow control**: Jump tables or function call verification

**Example approach**:
```C
// Simplified concept 
uint8_t sequence_counter = 0;
const uint8_t EXPECTED_SEQUENCE[] = {0x12, 0x34, 0x56, 0x78};

void Task1() { // e.g. ADC ISR
    if (sequence_counter != 0) Error();
    sequence_counter = EXPECTED_SEQUENCE[0];
    // task code
}

void Task2() { // e.g. sample processing
    if (sequence_counter != EXPECTED_SEQUENCE[0]) Error();
    sequence_counter = EXPECTED_SEQUENCE[1];
    // task code
}
```

### H.3 - Program Counter Test

**Purpose**: Verify the CPU program counter functions correctly.

**Requirements**:
- Test must verify program counter integrity
- Typically performed at startup

**Implementation methods**:
- Jump to known address and verify correct arrival
- Call/return sequence verification
- Address range checking

### H.4 - CPU Register Test

**Purpose**: Detect stuck bits or failures in CPU registers.

**Requirements**:
- Test critical CPU registers
- Must use complementary test patterns (0xAA, 0x55 or 0xFF, 0x00)
- Test frequency: At startup minimum, periodic for Class C

**Typical registers tested**:
- Accumulator
- General purpose registers
- Status/flag registers
- Stack pointer (if critical)

**Test pattern example**:
- Write 0xAA (10101010), read back and verify
- Write 0x55 (01010101), read back and verify
- Ensures no stuck-at-0 or stuck-at-1 faults

### H.5 - Clock Frequency Test

**Purpose**: Detect CPU clock frequency outside acceptable range.

**Requirements**:
- Must detect clock frequency variation beyond specified tolerance
- Typically ±15% to ±25% tolerance depending on application
- Test must be independent of system clock

**Implementation methods**:
- **Independent oscillator reference**: Use RC oscillator or crystal separate from main clock
- **Timeout measurement**: Measure known task duration against independent timebase
- **PLL monitoring**: For systems with PLLs, monitor lock status

**Typical approach**:
- Run independent timer during a known number of instruction cycles
- Compare elapsed time against acceptable window
- Flag error if outside tolerance

### H.6 - Interrupt Handling Test

**Purpose**: Verify interrupt system functions correctly.

**Requirements**:
- Interrupt generation and handling must be verified
- Critical interrupts must be tested
- Detect lost or spurious interrupts

**Implementation methods**:
- Generate test interrupts and verify they execute
- Monitor interrupt frequency for critical interrupts
- Timeout detection if expected interrupt doesn't occur
- Interrupt nesting level checks

### H.7 - Watchdog Function Test

**Purpose**: Ensure the watchdog timer itself functions correctly.

**Requirements**:
- The watchdog must be tested to ensure it will reset the system on failure
- Performed during initialization before entering normal operation

**Implementation**:
- Deliberately allow watchdog to expire during startup
- Verify reset occurred by checking reset reason flags
- Only proceed if watchdog demonstrated functionality

**Critical note**: This is testing the watchdog itself, not just using it for monitoring.

### H.11 - RAM Test

**Purpose**: Detect faults in RAM including stuck bits, address faults, and coupling faults.

**Requirements**:
- Must test all RAM used by safety-related software
- Use modified march test or equivalent
- Test patterns must include complementary patterns
- Class B: Test at startup
- Class C: Test at startup AND periodically during operation

**Common test algorithms**:

**March C algorithm** (widely used):
1. Write 0 to all locations (ascending)
2. Read 0, write 1 (ascending)
3. Read 1, write 0 (descending)
4. Read 0 (descending)

**March C+ algorithm** (more comprehensive):
- Adds additional pass for better coupling fault detection

**Checkerboard pattern**:
- Alternating 0xAA and 0x55 patterns
- Tests for address line faults

**Implementation considerations**:
- Critical data must be backed up before testing
- Can segment RAM and test sections over time for periodic testing
- Stack usage during test must be carefully managed

### H.12 - Non-Volatile Memory Test

**Purpose**: Verify program code and constants stored in Flash/ROM/EEPROM have not been corrupted.

**Requirements**:
- Entire program memory must be verified
- Typically uses CRC or checksum
- Class B: Test at startup
- Class C: Test at startup AND periodically during operation (at least once per 24 hours or more frequently based on risk assessment)

**Implementation approaches**:

**CRC-32** (recommended):
- Provides strong error detection
- Pre-calculated CRC stored in memory
- Runtime calculation compared against stored value

**Multiple checksums**:
- Segment memory and calculate individual checksums
- Allows localization of faults
- Reduces single-point-of-failure risk

**Diverse algorithms**:
- For highest integrity, use two different checksum algorithms
- Reduces risk of systematic errors in checking mechanism

**Runtime implementation**:
- Background CRC calculation (segment per cycle)
- Complete verification within specified time window
- Immediate verification of critical sections at startup

### H.13 - EEPROM/Data Memory Test

**Purpose**: Verify integrity of stored calibration data, configuration, and safety parameters.

**Requirements**:
- Critical data must have integrity check (CRC, checksum, parity)
- Redundant storage for critical parameters (store twice, compare on read)
- Default safe values if corruption detected

**Implementation**:
- Store CRC with each data block
- Verify on read, periodically, or both
- Range checking on retrieved values
- Write verification (read-back after write)

### H.15 - ADC Test

**Purpose**: Verify Analog-to-Digital Converter operates within specification.

**Requirements**:
- Test ADC accuracy and functionality
- Typically uses reference voltage or known voltage source

**Implementation methods**:
- **Internal reference test**: Measure internal bandgap or Vref
- **Ratiometric test**: Compare multiple ADC channels
- **Known voltage test**: Measure voltage divider from supply
- **Plausibility checks**: Range and rate-of-change verification

### H.16 - Output Test

**Purpose**: Verify output signals reach their intended states.

**Requirements**:
- Critical outputs must be monitored
- Detect stuck-at faults and short circuits

**Implementation**:
- **Feedback reading**: Read output state through separate input pin
- **Current sensing**: Monitor output current for load verification
- **Complementary outputs**: Use paired outputs that should be opposite states
- **Temporal checks**: Verify outputs change when commanded

### H.27 - Common Cause Failure Analysis

**Purpose**: Ensure test mechanisms themselves don't have common mode failures.

**Requirements**:
- Self-test mechanisms must be designed to avoid common cause failures with the functions they test
- Use diverse techniques where possible
- Independent resources for safety monitoring

## Test Frequencies

**Class B minimum requirements**:
- All H.2-H.27 tests at power-up/initialization
- Continuous or periodic tests for program sequence (H.2)
- Watchdog continuously active

**Class C requirements**:
- All Class B requirements PLUS:
- Periodic RAM test during operation (H.11)
- Periodic Flash/ROM test during operation (H.12)
- More frequent execution of other tests based on risk assessment
- Typically requires test completion within operational cycle or specified time period

## Typical Test Schedule Example

**At Startup** (before entering main loop):
1. Watchdog function test (H.7)
2. CPU register test (H.4)
3. Program counter test (H.3)
4. Full RAM test (H.11)
5. Full Flash CRC check (H.12)
6. Clock test initialization (H.5)
7. Interrupt test setup (H.6)
8. ADC test (H.15)
9. Output test (H.16)

**During Operation** (main loop):
- Program sequence test - every cycle (H.2)
- Watchdog refresh - every cycle (H.7)
- Clock frequency test - continuous/periodic (H.5)
- Interrupt monitoring - continuous (H.6)
- Partial Flash CRC - segmented over time (H.12)
- Partial RAM test - segmented or periodic (H.11) [Class C]
- ADC plausibility - with each reading (H.15)
- Output verification - with each change (H.16)

## Documentation Requirements

EN 60730 requires comprehensive documentation:

**Software Safety Plan**: Overall approach to achieving safety
**Software Requirements Specification**: Detailed safety requirements
**Software Design Specification**: Architecture and detailed design
**Test Plans and Results**: Verification of all H-tests
**Failure Mode Analysis**: FMEA covering software components
**Validation Report**: Evidence that requirements are met

## Compliance Verification

Third-party testing laboratories verify compliance through:
- Review of documentation
- Code inspection
- Fault insertion testing (injecting faults to verify detection)
- Environmental testing (temperature, EMC)
- Endurance testing

## Relationship to Other Standards

**IEC 61508 comparison**:
- EN 60730 Class B ≈ SIL 1-2
- EN 60730 Class C ≈ SIL 2-3 (for specific hazards)

**Key differences**:
- EN 60730 is more prescriptive with specific test methods
- IEC 61508 is more flexible but requires systematic capability demonstration
- EN 60730 focuses on hardware-software interface testing
- IEC 61508 has broader scope including systematic failure prevention

## Common Implementation Challenges

**Challenge 1 - RAM testing during operation**: Must preserve critical data while testing
**Solution**: Segment RAM, backup critical sections, test non-critical areas in rotation

**Challenge 2 - Flash CRC time**: Full CRC calculation takes too long
**Solution**: Segment memory, calculate portions over multiple cycles, track completion

**Challenge 3 - Watchdog test at startup**: Requires deliberate system hang
**Solution**: Implement early in boot before critical initializations, document timing carefully

**Challenge 4 - Independent clock reference**: Limited timers available
**Solution**: Use RC oscillator, external crystal, or timer in different clock domain

Would you like detailed implementation examples for any specific test requirement, or information about how to structure the safety architecture to meet these requirements efficiently?
