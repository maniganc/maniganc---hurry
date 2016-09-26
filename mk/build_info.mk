
BUILD_INFO_COMMIT := $(shell git log -1 --pretty=short 2>/dev/null | tr '\n' ' ')
ifeq ($(strip $(BUILD_INFO_COMMIT)),)
BUILD_INFO_COMMIT := "none"
endif

BUILD_INFO_DATE := "$(shell date)"

BUILD_INFO_CFLAGS = \
-DBUILD_INFO_COMMIT="$(BUILD_INFO_COMMIT)" \
-DBUILD_INFO_DATE=$(BUILD_INFO_DATE)

