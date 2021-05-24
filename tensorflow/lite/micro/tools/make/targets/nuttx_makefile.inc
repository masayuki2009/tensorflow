# Settings for NuttX based platforms
# For nuttx, Tensorflow lite Microcontroller is used as a library.

# This setting makefile accept 4 optional parameters on the make command line.
# These options below are needed for build an example of Tensorflow Microcontroller.
# But just build a library, no need to add those options.
#
#   NUTTX_DEFS       : This is the file path to Make.defs which includes configuration
#                          parameters of nuttx.
#   NUTTX_CONFIG_H   : This is the file path to config.h which includes configuration
#                          parameters for source code.
#   NUTTX_CURDIR     : This is the directory path of externals/tensorflow in nuttx
#                          source repository.
#   NUTTX_APP_TFMAKE : This is the file path to makefile.inc for additional source code
#                          in nuttx to use tensorflow.

# Evacuate Compiler flags to avoid override them with loading NuttX Config
TMP_CXXFLAGS := $(CXXFLAGS)
TMP_CCLAGS := $(CCFLAGS)

# Define empty variable for add nuttx specific settings
NUTTX_PLATFORM_FLAGS :=

ifneq ($(NUTTX_DEFS),)

# Load NuttX Config
include $(NUTTX_DEFS)

NUTTX_PLATFORM_FLAGS := \
  -DNUTTX_CONFIG_H="\"$(NUTTX_CONFIG_H)\"" \
  -I$(NUTTX_CURDIR)/wrapper_include

# Load application for Tensorflow lite micro in NuttX
ifneq ($(NUTTX_APP_TFMAKE),)
ifeq ($(CONFIG_EXTERNALS_TENSORFLOW_EXAMPLE_NONE),y)
-include $(NUTTX_APP_TFMAKE)
endif
endif

endif

TARGET_ARCH := cortex-m4
TARGET_TOOLCHAIN_PREFIX := $(CROSSDEV)

PLATFORM_FLAGS = \
  $(NUTTX_PLATFORM_FLAGS) \
  $(ARCHCPUFLAGS) \
  -DGEMMLOWP_ALLOW_SLOW_SCALAR_FALLBACK \
  -DTF_LITE_MCU_DEBUG_LOG \
  -fno-exceptions \
  -funsigned-char \
  -MMD \
  -Wall \
  -Wextra \
  -Wno-shadow \
  -Wno-vla \
  -Wno-strict-aliasing \
  -Wno-type-limits \
  -Wno-unused-parameter \
  -Wno-missing-field-initializers \
  -Wno-write-strings \
  -Wno-sign-compare \
  -Wunused-function \
  -fno-delete-null-pointer-checks \
  -fomit-frame-pointer \
  -Os

CXXFLAGS := $(TMP_CXXFLAGS) $(PLATFORM_FLAGS) -fno-use-cxa-atexit
CCFLAGS := $(TMP_CCFLAGS) $(PLATFORM_FLAGS)

BUILD_TYPE := micro

INCLUDES += -isystem$(MAKEFILE_DIR)/downloads/cmsis/CMSIS/Core/Include/

THIRD_PARTY_CC_SRCS := \
    $(THIRD_PARTY_CC_SRCS) \
    $(MAKEFILE_DIR)/../../nuttx/compiler_specific.cc \

# TODO: Now NuttX environment is not support tests.
#       So remove every tests.
#MICROLITE_TEST_SRCS :=
#MICRO_LITE_EXAMPLE_TESTS :=

# These are microcontroller-specific rules for converting the ELF output
# of the linker into a binary image that can be loaded directly.
OBJCOPY := $(TARGET_TOOLCHAIN_PREFIX)objcopy

$(BINDIR)/%.bin: $(BINDIR)/%
	@mkdir -p $(dir $@)
	$(OBJCOPY) $< $@ -O binary

