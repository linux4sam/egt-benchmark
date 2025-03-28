AUTOMAKE_OPTIONS = subdir-objects

CUSTOM_CXXFLAGS = $(WARN_CFLAGS) \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/external/hayai/src \
	$(LIBEGT_CFLAGS) \
	$(CAIRO_CFLAGS) \
	$(LIBINPUT_CFLAGS)

CUSTOM_LDADD = $(LIBEGT_LIBS) $(CAIRO_LIBS) $(LIBINPUT_LIBS) -ludev

bin_PROGRAMS = benchmark
benchmark_SOURCES = src/main.cpp
benchmark_CPPFLAGS = -I$(top_srcdir)/external/hayai/src -pthread
benchmark_CXXFLAGS = $(CUSTOM_CXXFLAGS) $(AM_CXXFLAGS) -DBENCHMARKDATA=\"$(datadir)/egt/benchmark\"
benchmark_LDADD = $(CUSTOM_LDADD)
benchmark_LDFLAGS = $(AM_LDFLAGS)

benchmarkdir = $(prefix)/share/egt/benchmark
dist_benchmark_DATA = $(wildcard $(top_srcdir)/images/*.png)
