
#$(OBJECTS) : defs.h
#kbd.o command.o files.o : command.h
#display.o insert.o search.o files.o : buffer.h

main.o : main.c defs.h
# $(COMPILE_CMD) -c $< -o $@

kbd.o : kbd.c defs.h command.h
	$(COMPILE_CMD) -c $< -o $@

command.o : command.c defs.h command.h
	$(COMPILE_CMD) -c $< -o $@

display.o : display.c defs.h buffer.h
	$(COMPILE_CMD) -c $< -o $@

insert.o : insert.c defs.h buffer.h
	$(COMPILE_CMD) -c $< -o $@

search.o : search.c defs.h buffer.h
	$(COMPILE_CMD) -c $< -o $@

files.o : files.c defs.h buffer.h command.h
	$(COMPILE_CMD) -c $< -o $@

utils.o : utils.c defs.h
	$(COMPILE_CMD) -c $< -o $@
