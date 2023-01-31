#!/usr/bin/perl
use strict;

my $buff;
read (STDIN, $buff, $ENV{'CONTENT_LENGTH'});
my %param = map { /([^=]+)=(.+)/ } split (/&/, $buff);

print "Content-Type: text/palin\n\n";

if (exists $param{'value1'}) {
    print "you input $param{'value1'}.";
} else {
    print "not input!";
}
if (exists $param{'value2'}) {
    print "you input $param{'value2'}.";
} else {
    print "not input!";
}