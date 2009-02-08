#!/usr/bin/perl -w

my $galaxies=shift;
my $holes=shift;

printf "%g\n%g\n%g\n%g\n\n", rand(1.0), rand(1.0), 0.0, rand(0.1)+0.05;
printf "G\n%g\n%g\n%g\n%g\n%g\n%g\n%g\n\n"
, rand(1.0), rand(1.0), 0.0 # x y z
,rand(1e4)-0.5e4 ,rand(1e4)-0.5e4 ,0.0 # vx vy vz
, rand(1e12) # m
while ($galaxies--);

printf "H\n%g\n%g\n%g\n%g\n%g\n%g\n%g\n\n"
, rand(1.0), rand(1.0), 0.0 # x y z
, 0,0,0
, rand(1e12) # m
while ($holes--);
