# Makefile for nusmuk_audio

 lib.name = nusmuk-utils

 class.sources = \
 	line3.c \
 	pbank.c \
 	mtx_preset.c \
 	tab_downsample_max.c \
 	tab_downsample_min.c \
 	tab_downsample_average.c \
 	tab_downsample.c \
 	tab_upsample.c  \
 	tab_upsample_copy.c \
 	tab_max.c \
 	tab_min.c \
 	tab_integrate.c \
 	tab_derivate.c

 datafiles = \
 	pps.pd \
 	between.pd \
 	many_bang.pd \
 	rand_diff.pd \
 	filtered_random.pd \
 	randn.pd \
 	fmod.pd \
 	once.pd \
 	rnd_flow.pd \
 	lb.pd \
 	p.pd \
 	rnd_metro.pd \
 	lfo.pd \
 	bushmeat.pbank \
 	common.h \
 	pbank.h \
 	nusmuk-utils-meta.pd

 PDLIBBUILDER_DIR=pd-lib-builder/
 include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder

