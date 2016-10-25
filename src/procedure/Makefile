

all: procedures.h procedures.c

C_FILES = $(filter-out procedures.c,$(wildcard *.c))

procedures.h: $(C_FILES)
	echo "#include \"procedures_macro.h\"" > procedures.h
	grep "^Mg_PROCEDURE\((.*)\)" *.c | sed -r "s/.*(Mg_PROCEDURE\(.*\)).*/\1;/g" >> procedures.h
	echo "extern const Mg_procedure Mg_procedure_array[];" >> procedures.h

procedures.c: $(C_FILES)
	echo "#include \"procedures.h\"" > procedures.c
	echo "const Mg_procedure Mg_procedure_array[] = {" >> procedures.c
	grep "^Mg_PROCEDURE\((.*)\)" *.c | sed -r "s/.*Mg_PROCEDURE\((.+), *(.+)\).*/\{Mg_PROCEDURE_\1, \2\},/g" >> procedures.c
	echo "{NULL, NULL}};" >> procedures.c 
clean:
	rm procedures.h
	rm procedures.c
