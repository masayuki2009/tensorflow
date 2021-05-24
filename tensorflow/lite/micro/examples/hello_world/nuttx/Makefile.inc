# Settings for nuttx platform for Hello World example
# This should be read when the CONFIG_EXAMPLES_TFLDEMO option is selected
# in NuttX configuration.

$(warning CONFIG_EXAMPLES_TFLDEMO=$(CONFIG_EXAMPLES_TFLDEMO))

ifeq ($(TARGET), nuttx)
ifeq ($(CONFIG_EXAMPLES_TFLDEMO),y)

NUTTX_HELLO_WORLD_EXCLUDED_SRCS = \
    tensorflow/lite/micro/examples/hello_world/main.cc

NUTTX_HELLO_WORLD_SRCS = \
    $(filter-out $(NUTTX_HELLO_WORLD_EXCLUDED_SRCS),$(HELLO_WORLD_SRCS))

# In nuttx case, those file should be included into libtensorflow-microlite.
THIRD_PARTY_CC_SRCS += $(NUTTX_HELLO_WORLD_SRCS)

endif
endif
