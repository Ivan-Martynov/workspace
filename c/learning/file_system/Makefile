# Prevent make from trying to remake the makefile.
#.PHONY: Makefile

# Include makefile with setup for compiler options.
include ../../make_compiler_setup.mk

BIN_DIR := bin
OBJ_DIR := obj
SRC_DIR := src
INC_DIR := include

CFLAGS += -I $(INC_DIR)

# vpath variable: search for source/header files in the src/include folder.
# That way we can omit explicit directory part for files.
vpath %.c $(SRC_DIR)
vpath %.h $(INC_DIR)
vpath %.o $(OBJ_DIR)

SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
TARGETS := $(BIN_DIR)/$(BUILD)/directory_creator.exe
# $(BIN_DIR)/$(BUILD)/list_files.exe
# $(BIN_DIR)/$(BUILD)/display_files.exe
# $(BIN_DIR)/$(BUILD)/tests.exe

.PHONY : all
all : $(TARGETS)

$(BIN_DIR)/$(BUILD)/list_files.exe: $(OBJ_DIR)/list_files.o \
	$(OBJ_DIR)/ID3v2_tag_reader.o \
	$(OBJ_DIR)/ID3v2_tag_header_reader.o \
	$(OBJ_DIR)/ID3v2_frame_header_reader.o \
	$(OBJ_DIR)/ID3v2_text_frame_reader.o \
	$(OBJ_DIR)/mrvn_string_helper.o \
	$(OBJ_DIR)/file_name_validator.o \
	$(OBJ_DIR)/syncsafe_int_converter.o
	$(LINK_CMD) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/$(BUILD)/display_files.exe: $(OBJ_DIR)/display_files.o \
	$(OBJ_DIR)/mrvn_string_helper.o $(OBJ_DIR)/ID3v2_tag_reader.o \
	$(OBJ_DIR)/ID3v2_tag_header_reader.o \
	$(OBJ_DIR)/ID3v2_frame_header_reader.o \
	$(OBJ_DIR)/ID3v2_text_frame_reader.o \
	$(OBJ_DIR)/syncsafe_int_converter.o
	$(LINK_CMD) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/$(BUILD)/directory_creator.exe: $(OBJ_DIR)/directory_creator.o \
	$(OBJ_DIR)/mrvn_string_helper.o
	$(LINK_CMD) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/$(BUILD)/tests.exe: $(OBJ_DIR)/tests.o \
	$(OBJ_DIR)/mrvn_string_helper.o \
	$(OBJ_DIR)/mrvn_map_char.o
	$(LINK_CMD) $^ -o $@ $(LDFLAGS)

#$(TARGETS) : $(OBJECTS)
#	$(LINK_CMD) $^ -o $@ $(LDFLAGS)

#$(OBJ_DIR)/ID3v2_tag_header_reader.o : $(SRC_DIR)/ID3v2_tag_header_reader.c \
#	$(INC_DIR)/ID3v2_tag_header_reader.h
#	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/display_files.o: $(SRC_DIR)/display_files.c \
	$(INC_DIR)/ID3v2_tag_reader.h \
	$(INC_DIR)/mrvn_string_helper.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/directory_creator.o: $(SRC_DIR)/directory_creator.c \
	$(INC_DIR)/mrvn_string_helper.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/list_files.o : $(SRC_DIR)/list_files.c $(INC_DIR)/ID3v2_tag_reader.h \
	$(INC_DIR)/file_name_validator.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/tests.o : $(SRC_DIR)/tests.c $(INC_DIR)/mrvn_map_char.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/ID3v2_tag_reader.o : $(SRC_DIR)/ID3v2_tag_reader.c \
	$(INC_DIR)/ID3v2_tag_reader.h \
	$(INC_DIR)/ID3v2_tag_header_reader.h \
	$(INC_DIR)/ID3v2_frame_header_reader.h \
	$(INC_DIR)/ID3v2_text_frame_reader.h \
	$(INC_DIR)/ID3v2_text_frame_ids.h \
	$(INC_DIR)/syncsafe_int_converter.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/ID3v2_tag_header_reader.o : $(SRC_DIR)/ID3v2_tag_header_reader.c \
	$(INC_DIR)/ID3v2_tag_header_reader.h \
	$(INC_DIR)/syncsafe_int_converter.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/ID3v2_frame_header_reader.o : $(SRC_DIR)/ID3v2_frame_header_reader.c \
	$(INC_DIR)/ID3v2_frame_header_reader.h \
	$(INC_DIR)/syncsafe_int_converter.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/ID3v2_text_frame_reader.o : $(SRC_DIR)/ID3v2_text_frame_reader.c \
	$(INC_DIR)/ID3v2_text_frame_reader.h \
	$(INC_DIR)/mrvn_string_helper.h \
	$(INC_DIR)/syncsafe_int_converter.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/mrvn_map_char.o : $(SRC_DIR)/mrvn_map_char.c \
	$(INC_DIR)/mrvn_map_char.h \
	$(INC_DIR)/mrvn_string_helper.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/mrvn_string_helper.o : $(SRC_DIR)/mrvn_string_helper.c \
	$(INC_DIR)/mrvn_string_helper.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/file_name_validator.o : $(SRC_DIR)/file_name_validator.c \
	$(INC_DIR)/file_name_validator.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/syncsafe_int_converter.o : $(SRC_DIR)/syncsafe_int_converter.c \
	$(INC_DIR)/syncsafe_int_converter.h
	$(CC) -c $(CFLAGS) $< -o $@

# $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(INC_DIR)/%.h
# 	$(CC) -c $(CFLAGS) $< -o $@

$(OBJECTS) : | $(OBJ_DIR)

$(OBJ_DIR) : ; mkdir $(OBJ_DIR)

$(TARGETS) : |$(BIN_DIR)

$(BIN_DIR) : ; mkdir -p $(BIN_DIR)/$(BUILD)

.PHONY : clean
clean : ; rm $(OBJECTS) $(TARGETS)
