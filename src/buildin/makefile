

all: buildin_procedures.h buildin_procedures.c

C_FILES = $(filter-out buildin_procedures.c,$(wildcard *.c))

buildin_procedures.h: $(C_FILES)
	echo "#include \"buildin_procedure_macro.h\"" > buildin_procedures.h
	grep "^MG_BUILDIN_PROCEDURE\((.*)\)" *.c | sed -r "s/.*(MG_BUILDIN_PROCEDURE\(.*\)).*/\1;/g" >> buildin_procedures.h
	echo "extern const Mg_buildin_procedure Mg_buildin_procedure_array[];" >> buildin_procedures.h

buildin_procedures.c: $(C_FILES)
	echo "#include \"buildin_procedures.h\"" > buildin_procedures.c
	echo "const Mg_buildin_procedure Mg_buildin_procedure_array[] = {" >> buildin_procedures.c
	grep "^MG_BUILDIN_PROCEDURE\((.*)\)" *.c | sed -r "s/.*MG_BUILDIN_PROCEDURE\((.+), *(.+)\).*/\{mg_buildin_procedure_\1, \2\},/g" >> buildin_procedures.c
	echo "{NULL, NULL}};" >> buildin_procedures.c 
clean:
	rm buildin_procedures.h
	rm buildin_procedures.c
