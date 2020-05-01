AM_CPPFLAGS = -I$(top_srcdir)/googletest/googletest/include/ -I$(top_srcdir)/googletest/googlemock/include
AM_LDFLAGS = -L$(top_srcdir)/googletest/googletest/lib/.libs/ -L$(top_srcdir)/googletest/googlemock/lib/.libs/
LDADD = -lpthread -lgtest
bin_PROGRAMS = test 
test_SOURCES = test.cpp
TESTS = test
