#!/usr/bin/perl
use strict;

my $buff;
read (STDIN, $buff, $ENV{'CONTENT_LENGTH'});
my %param = map { /([^=]+)=(.+)/ } split /&/, $buff;

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