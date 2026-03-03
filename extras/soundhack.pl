#!/usr/bin/perl
use Cwd;
use Cwd 'abs_path';
use Cwd qw(chdir);

#chdir("../assets/audio/");
$framelength = @ARGV[0];#max 32 "20"in hex

my $outfile = "../assets/audio/perlTest.sfx";

if (!@ARGV) {
    print "using framelength 8\n";
    $framelength = 8;
#exit;
}
else
{
    # $framelength = @ARGV[0];
    if ($framelength > 32) {$framelength = 32;};
    print ("framelength set to ". $framelength ."\n");
};
#
#$test = @ARGV[0];
#text file test
my $output = "0800 0007 0606 2a2a 2a2a 0007 0606 2a2a
2a2a 0007 0606 2a2a 2a2a 0007 0606 2a2a
2a2a 0007 0606 2a2a 2a2a 0007 0606 2a2a
2a2a 0007 0606 2a2a 2a2a 0007 0606 1010
1010";


# open(my $filehandle, '>', $outfile) or die "Could not open file: $!";
# print $filehandle $output;
# close $filehandle;

#binary test
open(my $out, '>:raw', $outfile) or die "Unable to open: $!";
#print $out pack('s<', 255 );
my $hex = sprintf("0x%X", $framelength);
my $firstbyte = ($hex."00");
print $out pack('s<', hex($hex) );

#print $out pack('s>', hex($hex));

for ($i=0; $i<8;$i++){
    print $out pack('s>', hex("0007") );
    print $out pack('s>', hex("0606") );
    print $out pack('s>', hex("2a2a") );
    print $out pack('s>', hex("2a2a") );
};
# print $out pack('s<', 39);
# print $out pack('s<', 255);
# print $out pack('s<', 39);
close($out);


#sfx file format
#0800 0007 0606 2a2a 2a2a 0007 0606 2a2a
#2a2a 0007 0606 2a2a 2a2a 0007 0606 2a2a
#2a2a 0007 0606 2a2a 2a2a 0007 0606 2a2a
#2a2a 0007 0606 2a2a 2a2a 0007 0606 1010
#1010 


exit 0; 