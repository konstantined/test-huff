#!/bin/sh
#\
exec wish8.3 "$0" "$@"

# Added in Branch-01

set grad 0.01
set count [expr [lindex $argv 0] / $grad ]

set i 0
while { $i < $count } {
  incr i
  after [expr round(0.2 / $grad)]
  puts [ expr $i * $grad ]
}

exit

