AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I`root-config --incdir`

AM_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -L$(OPT_SPHENIX)/lib

lib_LTLIBRARIES = \
  libphhepmc_io.la \
  libphhepmc.la

pkginclude_HEADERS = \
  Fun4AllHepMCInputManager.h \
  Fun4AllHepMCOutputManager.h \
  Fun4AllOscarInputManager.h \
  PHGenEvent.h \
  PHGenEventv1.h \
  PHGenEventList.h \
  PHGenEventListv1.h \
  PHHepMCGenEvent.h

libphhepmc_la_LDFLAGS = \
  ${AM_LDFLAGS} \
  `root-config --libs`

libphhepmc_la_LIBADD = \
  libphhepmc_io.la \
  -lphool \
  -lSubsysReco \
  -lboost_iostreams \
  -lfun4all

libphhepmc_la_SOURCES = \
  Fun4AllHepMCInputManager.cc \
  Fun4AllHepMCInputManager_Dict.C \
  Fun4AllHepMCOutputManager.cc \
  Fun4AllHepMCOutputManager_Dict.C \
  Fun4AllOscarInputManager.cc \
  Fun4AllOscarInputManager_Dict.C \
  PHHepMCParticleSelectorDecayProductChain.cc \
  PHHepMCParticleSelectorDecayProductChain_Dict.C

libphhepmc_io_la_SOURCES = \
  PHGenEventv1.cc \
  PHGenEventListv1.cc \
  PHHepMCGenEvent.cc \
  PHHepMC_io_Dict.C

libphhepmc_io_la_LDFLAGS = \
  ${AM_LDFLAGS} \
  `root-config --libs`

libphhepmc_io_la_LIBADD = \
  -lphool \
  -lHepMC

BUILT_SOURCES = \
  testexternals.cc

noinst_PROGRAMS = \
  testexternals_libphhepmc \
  testexternals_libphhepmc_io

testexternals_libphhepmc_SOURCES = testexternals.cc
testexternals_libphhepmc_LDADD = libphhepmc.la

testexternals_libphhepmc_io_SOURCES = testexternals.cc
testexternals_libphhepmc_io_LDADD = libphhepmc_io.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

# Rule for generating table CINT dictionaries.
%_Dict.C: %.h %LinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

PHHepMC_io_Dict.C: \
  PHGenEvent.h \
  PHGenEventv1.h \
  PHGenEventList.h \
  PHGenEventListv1.h \
  PHHepMCGenEvent.h \
  $(OFFLINE_MAIN)/include/HepMC/*.h \
  PHHepMCLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

clean-local:
	rm -f *Dict* $(BUILT_SOURCES)
