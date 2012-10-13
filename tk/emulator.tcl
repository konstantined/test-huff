#!/bin/sh
#\
exec wish8.3 "$0" "$@"

###############################
proc dialogMsg {} {
	set i [ tk_dialog .dialog1 "Dialog example" "Форматитруем диск.\nВы уверены?" "" 0 "Пофиг" "Нефиг" ]
	switch $i {
		0 { puts "OK" }
		1 { puts "Cancel" }
	}
}
###############################
proc showTrk { id } {
  set ftrk "ftrk$id"
  frame .$ftrk -width 100 -height 100 -bd 3 -relief groove
  label .$ftrk.l -text "ТРК $id" -anchor w
  .$ftrk.l config -bg wheat

  frame .$ftrk.summa
  label .$ftrk.summa.l -text "Сумма" -width 6 -anchor w
  label .$ftrk.summa.val -textvariable summa$id -width 10 -anchor e
  pack .$ftrk.summa.l .$ftrk.summa.val -side left -expand 1 -fill x
  
  frame .$ftrk.littr
  label .$ftrk.littr.l -text "Литры" -width 6 -anchor w
  label .$ftrk.littr.val -textvariable littr$id -width 10 -anchor e
  pack .$ftrk.littr.l .$ftrk.littr.val -side left -expand 1 -fill x
  
  frame .$ftrk.price
  label .$ftrk.price.l -text "Цена" -width 6 -anchor w
  label .$ftrk.price.val -textvariable price$id  -width 10 -anchor e
  pack .$ftrk.price.l .$ftrk.price.val -side left -expand 1 -fill x

  frame .$ftrk.width -height 1 -bd 0 -width 140
  frame .$ftrk.ind -width 140 -height 8 -bd 2 -relief raised -bg darkblue

  foreach field { summa littr price } {
    .$ftrk.$field.l configure -fg yellow -bg black
    .$ftrk.$field.val configure -fg white -bg black
  }
  label .$ftrk.fuel -textvariable fuel$id
  .$ftrk.fuel config -bg darkgreen
  frame .$ftrk.btns -bg wheat
  button .$ftrk.btns.fillStart -text "Заправка" -command " fillStartDialog $id "
  button .$ftrk.btns.fillStop -text "Стоп" -command " Stop $id " -state disabled
  pack .$ftrk.btns.fillStart .$ftrk.btns.fillStop -side left -expand 1 -fill x
#  pack .$ftrk.l .$ftrk.summa .$ftrk.littr .$ftrk.price .$ftrk.fuel .$ftrk.btns -side top -fill x
  pack .$ftrk.l .$ftrk.summa .$ftrk.littr .$ftrk.price -side top -fill x
  pack .$ftrk.ind .$ftrk.width -side top -anchor w
  pack .$ftrk.fuel .$ftrk.btns -side top -fill x
}
###############################
proc littr2summa { littr price } {
  if { [string length $littr] > 0 } {
    set summa [format %.2f [expr $littr * $price]]
  } else {
    set summa ""
  }
  return $summa
}
proc summa2littr { summa price } {
  if { [string length $summa] > 0 } {
    set littr [format %.2f [expr ($summa + 0.0) / $price]]
  } else {
    set littr ""
  }
  return $littr
}
###############################
proc fillStartDialog { trk_id } {
  global filltypes paytypes fuels cur_trk_id
  set fuels_num [expr [array size fuels] / 2 - 1]
  toplevel .fillparams -bd 10
  wm title .fillparams "Параметры заправки"
  label .fillparams.l -text "Параметры заправки" -bd 5
  frame .fillparams.f
  frame .fillparams.f.fueltype -bd 2 -relief groove
  frame .fillparams.f.filltype -bd 2 -relief groove
  frame .fillparams.f.paytype -bd 2 -relief groove
  frame .fillparams.e -bd 10
  label .fillparams.e.littrL -text "Литры"
  label .fillparams.e.summaL -text "Сумма" -fg #999999
  entry .fillparams.e.littrE -width 10 -textvariable fill_littr \
                                       -validate all -vcmd {string is double %P} \
                                       -fg "black" -selectbackground "black" -selectforeground "white"
  entry .fillparams.e.summaE -width 10 -state disabled -textvariable fill_summa \
                                       -validate all -vcmd {string is double %P} \
                                       -fg "black" -selectbackground "black" -selectforeground "white"
  frame .fillparams.btns -bd 10
  label .fillparams.f.fueltype.l -text "Продукт"
  label .fillparams.f.filltype.l -text "Заправка"
  label .fillparams.f.paytype.l -text "Оплата"
  pack .fillparams.e.littrL .fillparams.e.littrE .fillparams.e.summaL .fillparams.e.summaE -side left -anchor w
  pack .fillparams.f.fueltype.l .fillparams.f.filltype.l .fillparams.f.paytype.l
  pack .fillparams.f.fueltype .fillparams.f.filltype .fillparams.f.paytype -side left -anchor n -padx 10
  pack .fillparams.l .fillparams.f .fillparams.e .fillparams.btns -side top
  pack .fillparams.l .fillparams.f .fillparams.btns -side top

  for { set i 0 } { $i <= $fuels_num } { incr i } {
    radiobutton .fillparams.f.fueltype.rb$i -text  $fuels($i,name) \
					-relief flat -font "fixed" \
					-variable fueltype \
					-value $i
    pack .fillparams.f.fueltype.rb$i -anchor w
  }
  foreach i { 0 1 2 } {
    radiobutton .fillparams.f.filltype.rb$i -text [ lindex $filltypes $i ] \
					-relief flat -font "fixed" \
					-variable filltype \
					-value $i
    pack .fillparams.f.filltype.rb$i -anchor w
    .fillparams.f.filltype.rb$i configure -command "activateEntry $i"
  }
  foreach i { 0 1 } {
    radiobutton .fillparams.f.paytype.rb$i -text [ lindex $paytypes $i ] \
					-relief flat -font "fixed" \
					-variable paytype \
					-value [ lindex $paytypes $i ] -font "fixed"
    pack .fillparams.f.paytype.rb$i -anchor w
  }
  .fillparams.f.fueltype.rb0 select
  .fillparams.f.filltype.rb0 select
  .fillparams.f.paytype.rb0 select
  button .fillparams.btns.ok -text "Начать" -command "Run $trk_id"
  button .fillparams.btns.cancel -text "Отмена" -command { destroy .fillparams }
  pack .fillparams.btns.ok .fillparams.btns.cancel -side left -padx 20
  .fillparams.btns.ok configure -state disabled
  focus .fillparams.btns.cancel
  grab .fillparams
  for { set i 0 } { $i <= $fuels_num } { incr i } {
    bind .fillparams.f.fueltype.rb$i <ButtonRelease> {
      if {[string length $fill_littr] > 0} {
      if { $filltype == 0 } {
        set fill_summa [littr2summa $fill_littr $fuels($fueltype,price)]
      }
      if { $filltype == 1 } {
        set fill_littr [summa2littr $fill_summa $fuels($fueltype,price)]
      }
      }
    }
  }
  bind .fillparams.f.filltype.rb2 <ButtonPress> {
    set fill_littr "Полный бак"
    set fill_summa ""
    activateEntry 2
  }
  bind .fillparams.e.littrE <KeyRelease> {
    set fill_summa [littr2summa $fill_littr $fuels($fueltype,price)]
    setRunBtnStatus
  }
  bind .fillparams.e.littrE <B2-ButtonRelease> {
    puts Button-2
  }
  bind .fillparams.e.summaE <KeyRelease> {
    set fill_littr [summa2littr $fill_summa $fuels($fueltype,price)]
    setRunBtnStatus
  }
  set cur_trk_id $trk_id
  bind .fillparams <Return> { if { $fill_summa > 0 || $filltype == 2 } { Run $cur_trk_id } }
  bind .fillparams <Escape> { destroy .fillparams }
}

proc setRunBtnStatus {} {
  global fill_summa fill_littr filltype
    if { ([string length $fill_summa] > 0 && $fill_littr > 0) || $filltype == 2 } {
      .fillparams.btns.ok configure -state normal
    } else {
      .fillparams.btns.ok configure -state disabled
    }
}

proc activateEntry { i } {
  global fill_littr fill_summa
  switch $i {
    0 { .fillparams.e.summaL configure -fg #999999
        .fillparams.e.summaE configure -state disabled
        .fillparams.e.littrL configure -fg black
        .fillparams.e.littrE configure -state normal
        if {$fill_littr == "Полный бак"} {
          set fill_littr ""
        }
        focus .fillparams.e.littrE
      }
    1 { .fillparams.e.littrL configure -fg #999999
        .fillparams.e.littrE configure -state disabled
        .fillparams.e.summaL configure -fg black
        .fillparams.e.summaE configure -state normal
        if {$fill_littr == "Полный бак"} {
          set fill_littr ""
        }
        focus .fillparams.e.summaE
      }
    2 { .fillparams.e.littrL configure -fg #999999
        .fillparams.e.littrE configure -state disabled
        .fillparams.e.summaL configure -fg #999999
        .fillparams.e.summaE configure -state disabled
        focus .fillparams.btns.ok
      }
  }
  setRunBtnStatus
}
###############################
proc butrk {} {
  foreach trk_id { 1 2 3 4} {
    set f "ftrk$trk_id"
    showTrk $trk_id
    pack .$f -side left
  }
}
proc trkReset { id } {
  global trk littr$id summa$id fill_littr_id
  set ftrk "ftrk$id"
  set fuel$id ""
  set price$id 0.00
  set summa$id 0.00
  set littr$id 0.00
  .$ftrk.fuel config -bg "red" -fg "white"
  .$ftrk.ind configure -width 0
}
###############################
proc Run { id } {
  global input filltype filltypes paytype fueltype fill_littr fill_summa fuels fuel$id price$id fuel_id filltype_id paytype_id fill_littr_id fill_summa_id
  set filltype_id($id) [lindex $filltypes $filltype]
  if { $filltype == 2 } { set fill_littr 1000000 }
  set fill_littr_id($id) $fill_littr
  set fill_summa_id($id) $fill_summa
  set fuel_id($id) $fueltype
  set paytype_id($id) $paytype
  set cmd "./timer.tcl $fill_littr_id($id)"
  trkReset $id
  if [catch {open "| $cmd"} input($id)] {
    $log insert end $input($id)\n
  } else {
    fileevent $input($id) readable "Log $id"
    set f "ftrk$id"
    destroy .fillparams
    .$f.fuel config -bg "red" -fg "white"
    set fuel$id $fuels($fuel_id($id),name)
    set price$id [format %.2f $fuels($fuel_id($id),price)]
    .$f.btns.fillStart config -state disabled
    .$f.btns.fillStop config -state normal
    puts "Start filling: TRK N $id; FillType=$filltype, FillMode=$paytype, Fuel=$fuels($fueltype,name); Order: l=$fill_littr_id($id), s=$fill_summa_id($id)"
  }
  set fill_littr ""
  set fill_summa ""
}
proc Log { id } {
  global input trk littr$id summa$id fuels fuel_id filltype_id paytype_id fill_littr_id fill_summa_id
  if [eof $input($id)] {
    Stop $id
  } else {
    gets $input($id) line
    if { $line > 0 } {
      set summa$id [ format %.2f [ expr $line * $fuels($fuel_id($id),price) ] ]
      set littr$id [ format %.2f $line ]
      set ftrk "ftrk$id"
      if { $filltype_id($id) == "До полного бака" } {
        set ind 140
      } else {
        set ind [ expr ceil(($line + 0.0) / $fill_littr_id($id) * 140) ]
      }
      .$ftrk.ind configure -width $ind
#      puts "TRK N $id => $line, price = $fuels($fuel_id($id),price)"
    }
  }
}
proc Stop { id } {
  global input trk littr$id summa$id fuels fuel_id filltype_id paytype_id fill_littr_id fill_summa_id
  catch {close $input($id)}
  set f "ftrk$id"
  .$f.littr.val config -text 0
  .$f.fuel config -text "" -bg "darkgreen"
  .$f.btns.fillStart config -state normal
  .$f.btns.fillStop config -state disabled
  set price [format %.2f $fuels($fuel_id($id),price)]
  set litters [ set littr$id ]
  puts "Stop filling: TRK N $id, Price=$price,  Litters=$litters, FillType=$filltype_id($id), PayType=$paytype_id($id); Order: l=$fill_littr_id($id), s=$fill_summa_id($id)"
}
#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
###############################

set trk_num 4
set fuel_num 3

set fuels(0,name) "A-76" 
set fuels(0,price) 2.5
set fuels(1,name) "A-93" 
set fuels(1,price) 4.5
set fuels(2,name) "ДТ" 
set fuels(2,price) 10

set filltypes {"Литры" "На сумму" "До полного бака"}
set paytypes {"За наличные" "По карточке"}

for { set id 1 } { $id <= $trk_num } { incr id } {
  set summa$id 0.00
  set littr$id 0.00
  set price$id 0.00
  set fuel$id ""
}

wm title . "Эмулятор БУ ТРК"
. config -bg lightgrey

frame .fmain -bd 20
label .fmain.l -text "Наш БУ ТРК" -bg lightgrey -bd 20
pack .fmain.l

butrk

frame .fbtns -bd 20
.fbtns config -bg lightgrey
.fmain config -bg lightgrey
button .fbtns.exitBtn -text "Выход" -command { exit }
pack .fbtns.exitBtn -side left
pack .fmain .fbtns -side top


