
MAKEFILE_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

# clang-format command
CLANG_FORMAT := clang-format

# list of file that will be clang formatted
CLANG_FORMAT_DIR := \
	$(MAKEFILE_DIR)/bsp/a53_renode \
	$(MAKEFILE_DIR)/drivers/opena64 \
	$(MAKEFILE_DIR)/examples/hello_world \
	$(MAKEFILE_DIR)/examples/periodic_timer \
	$(MAKEFILE_DIR)/examples/unity_test

.PHONY: clang_format

clang_format:
	find $(CLANG_FORMAT_DIR) -type f \( -name '*.c' -o -name '*.h' \) -print0 \
		| xargs -0 $(CLANG_FORMAT) --style=file -i

