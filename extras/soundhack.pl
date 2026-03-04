#!/usr/bin/perl
use Cwd;
use Cwd 'abs_path';
use Cwd qw(chdir);

#chdir("../assets/audio/");
my $framelength = @ARGV[0];#max 32 "20"in hex
my $feedback = 5;#higher seems more mellow
my $frequency = @ARGV[1];# min 8 max 107 middleC C4 = 48
my $amplitude = 6;#7;#@ARGV[2]; #min 0 max 8 amplitudes template 0007 0606

my $outfile = @ARGV[2]; #"../assets/audio/perlTest.sfx";

if (!@ARGV) {
    print "Usage: \n";
    print "./soundhack.pl <framelength> <frequency> <filename>\n\n";
    print "framelength default 8\n";
    print "frequency default 48 (middle c, c4) \n";
    print "default file: ../assets/audio/perlTest.sfx\n\n";

    $framelength = 8;
    $frequency = 48;
    $outfile = "../assets/audio/perlTest.sfx";
    print "continue with defaults? (y/n): ";
    my $userword = <STDIN>; # I moved chomp to a new line to make it more readable
    chomp $userword; # Get rid of newline character at the end
    exit 0 if ($userword eq ""); # If empty string, exit.
    if ($userword == "y" || $userword == "Y")
    {} else {exit 0;};
}
else
{
    if ($framelength > 32) {$framelength = 32;};
    print ("framelength set to ". $framelength ."\n");
};

my $framelengthFeedback = 0;
$framelengthFeedback = $framelength + (256 * $feedback);
my $amp0 = 256 * $amplitude;
my $amp1 = ($amplitude - 1) * 257;
if ($amp1 < 0) {$amp1 = 0;};
$hexAmp0 = sprintf("0x%X", ($amp0));
$hexAmp1 = sprintf("0x%X", ($amp1));
#binary test
open(my $out, '>:raw', $outfile) or die "Unable to open: $!";

my $hex = sprintf("0x%X", $framelengthFeedback);
print $out pack('s<', hex($hex) );

my $hexFreq = sprintf("0x%X", ($frequency * 257)); #mult 257 to turn two bytes into four bytes

for ($i=0; $i<$framelength;$i++){
    #print $out pack('s>', hex("0007") );#Amp 0,1 ##first opamp sound like a modulator or boost, second seem like soft to hard tone
    #print $out pack('s>', hex("0606") );#Amp 2,3 # keep 6,6

    print $out pack('s<', hex($hexAmp0) );
    print $out pack('s<', hex($hexAmp1) );

    # print $out pack('s>', hex("2a2a") );#freq 0,1
    # print $out pack('s>', hex("2a2a") );#freq 0,1
    print $out pack('s<', hex($hexFreq) );
    print $out pack('s<', hex($hexFreq) );

};
close($out);


#sfx file format
#0800 0007 0606 2a2a 2a2a 0007 0606 2a2a
#2a2a 0007 0606 2a2a 2a2a 0007 0606 2a2a
#2a2a 0007 0606 2a2a 2a2a 0007 0606 2a2a
#2a2a 0007 0606 2a2a 2a2a 0007 0606 1010
#1010 


exit 0; 