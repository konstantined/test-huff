#!/bin/sh
#\
exec wish8.5 "$0" "$@"

# added in Master
XXXXXXXXXX

set grad 0.01
set count [expr [lindex $argv 0] / $grad ]

set i 0
while { $i < $count } {
  incr i
  after [expr round(0.2 / $grad)]
  puts [ expr $i * $grad ]
}

exit

