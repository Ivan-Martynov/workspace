PROJECT_NAME := Recursive_ver_10
PROJECT_VERSION_MAJOR := 1
PROJECT_VERSION_MINOR := 0
PROJECT_CONFIG_FILE := ${INC_DIR}/ProjectConfig.h
PROJECT_CONFIG_FILE_HEADER_GUARD := H_PROJECT_CONFIG_H

.PHONY: all ${PROJECT_CONFIG_FILE}

all: ${PROJECT_CONFIG_FILE}

# Using file function to write to file. Writing current directory.
${PROJECT_CONFIG_FILE}:
	${file > $@,#ifndef ${PROJECT_CONFIG_FILE_HEADER_GUARD}}
	${file >> $@,#define ${PROJECT_CONFIG_FILE_HEADER_GUARD}}
	${file >> $@,}
	${file >> $@,#define PROJECT_VERSION_MAJOR ${PROJECT_VERSION_MAJOR}}
	${file >> $@,#define PROJECT_VERSION_MINOR ${PROJECT_VERSION_MINOR}}
	${file >> $@,}
	${file >> $@,#define MAIN_DIR \}
	${file >> $@,	"${CURDIR}"}
	${file >> $@,}
	${file >> $@,#endif // ${PROJECT_CONFIG_FILE_HEADER_GUARD}}
