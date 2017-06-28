SHELL   = /bin/bash
CC      = gcc
CFLAGS  = -std=gnu11 -O2

INCLUDE  = include
OUTDIR   = bin
SRCDIR   = src
TARGETS  = $(addprefix ${OUTDIR}/,sort gen_input)

.PHONY: all
all: ${TARGETS}

${OUTDIR}/sort: $(wildcard ${SRCDIR}/sort/*.c) ${SRCDIR}/util/vector.c ${SRCDIR}/main.c
	${CC} ${CFLAGS} -I${INCLUDE} -o $@ $^

#${OUTDIR}/gen_input: ${SRCDIR}/util/gen_input.c
#	${CC} ${CFLAGS} -I${INCLUDE} -o $@ $^

${OUTDIR}/gen_input: ${SRCDIR}/util/gera_vets.c ${SRCDIR}/util/vetor.c
	${CC} ${CFLAGS} -I${INCLUDE} -o $@ $^

${TARGETS}: | ${OUTDIR}

${OUTDIR}:
	@mkdir -p $@

.PHONY: clean
clean:
	rm -r ${OUTDIR}
