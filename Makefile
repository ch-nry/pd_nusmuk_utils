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
 	between-help.pd \
 	line3-help.pd \
 	p.pd \
	rnd_metro.pd \
 	between.pd \
	many_bang-help.pd \
	pps-help.pd \
	tab_derivate-help.pd \
	filtered_random-help.pd \
	many_bang.pd \
	pps.pd \
	tab_downsample_average-help.pd \
	filtered_random.pd \
	_mtx_preset-help.pd \
	preset_param.pd \
	tab_downsample-help.pd \
	fmod-help.pd \
	mtx_preset-help.pd \
	rand_diff-help.pd \
	tab_downsample_max-help.pd \
	fmod.pd \
	_mtx_preset.pd \
	rand_diff.pd \
	tab_downsample_min-help.pd \
	img2cnv.pd \
	nusmuk-utils-meta.pd \
	randn-help.pd \
	tab_integrate-help.pd \
	lb-help.pd \
	once-help.pd \
	randn.pd \
	tab_max-help.pd \
	lb.pd \
	once.pd \
	rnd_flow-help.pd \
	tab_min-help.pd \
	lfo-help.pd \
	pbank-help.pd \
	rnd_flow.pd \
	tab_upsample_copy-help.pd \
	lfo.pd \
	p-help.pd \
	rnd_metro-help.pd \
	tab_upsample-help.pd \
	LICENCE.txt \
	README.txt \
	bushmeat.pbank


 PDLIBBUILDER_DIR=pd-lib-builder/
 include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder

