
#CFLAGS+= -pthread - assume this is already done for all linux platforms 


## Used for direct pigpio actions
LIB+=pigpio

## Used for pigpiod daemon
#todo 2025 this should be conditional on the daemon version. We probably never want it, but if we do should it be another hal support target or conditional here?
LIB+=pigpiod_if2
LIB+=rt
