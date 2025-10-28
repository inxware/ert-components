# Architectural Requirements

## Software diversity
For SIL 3 N-version programming becomes recommended to detect systematic faults.

## Failure detection
Enhanced diagnostic coverage requirements, typically 90-99% for SIL 3 (compared to 60-90% for SIL 2).

# Self-test Requirements

IEC61508 and related standards have specific requirements for memory (RAM/Flash), registers, and temporal processing integrity at SIL 2 and SIL 3.

## RAM (Volatile Memory) Protection

**SIL 2 Requirements**:
- Periodic RAM tests (march tests, checkerboard patterns)
- Stack overflow detection
- Address range checking
- Parity or ECC (Error Correcting Code) recommended

**SIL 3 Requirements**:
- ECC becomes highly recommended or required
- More frequent RAM testing cycles
- Multiple independent RAM test algorithms
- Enhanced stack monitoring with configurable limits

## Flash/Non-Volatile Memory Protection

**SIL 2 Requirements**:
- CRC or checksum verification of program code
- Verification at startup and periodically during operation
- Protection against unauthorized modifications

**SIL 3 Requirements**:
- Stronger integrity checks (CRC-32 or cryptographic hashes)
- More frequent runtime verification
- Dual checksums or diverse checking algorithms
- Memory segmentation with individual checksums per segment

## Register/CPU Integrity

**SIL 2 Requirements**:
- Basic CPU self-test at startup
- Periodic verification of critical registers
- Program flow monitoring

**SIL 3 Requirements**:
- More comprehensive CPU self-tests (testing ALU, registers, instruction set)
- Continuous or frequent register integrity checks
- Enhanced program flow monitoring with multiple checkpoints
- Instruction test patterns

## Temporal Processing (Timing) Requirements

**SIL 2 Requirements**:
- **Watchdog timers**: Independent watchdog to detect software hangs
- **Task monitoring**: Monitoring of critical task execution times
- **Timeout detection**: Detection of communication and processing timeouts
- **Sequence monitoring**: Basic verification that tasks execute in correct order

**SIL 3 Requirements**:
- **Multiple watchdogs**: Often requires two independent watchdog mechanisms
- **Window watchdog**: Not just maximum time but also minimum time checks (prevents too-fast execution)
- **Logical monitoring**: Verification of task execution sequence and dependencies
- **Arrival rate monitoring**: Detection of task over-runs or under-runs
- **Time-stamping**: Critical events time-stamped for temporal analysis
- **Temporal firewalls**: Prevention of timing interference between tasks

## Typical Diagnostic Coverage Targets

**SIL 2**: 60-90% diagnostic coverage for dangerous failures
**SIL 3**: 90-99% diagnostic coverage for dangerous failures

Memory and timing tests must detect 90-99% of potential dangerous faults at SIL 3.

## Common Implementation Approaches

**Memory testing patterns**:
- March C/C+ algorithms for RAM
- Galpat or Walking 1/0 tests
- Abraham test sequences

**Temporal monitoring**:
- Program flow monitoring with checksums
- Alive counters incremented at specific code points
- Execution time profiling for anomaly detection

Would you like specific implementation examples for any of these protection mechanisms?

