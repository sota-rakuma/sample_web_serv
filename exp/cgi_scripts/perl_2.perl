#!/usr/bin/perl
use strict;

my %param = map { /([^=]+)=(.+)/ } split /&/, $ENV{'QUERY_STRING'};

if (exists $param{'value'}) {
    print "you input $param{'value'}.";
} else {
    print "not input!";
}
if (exists $param{'value_2'}) {
    print "you input $param{'value_2'}.";
} else {
    print "not input!";
}