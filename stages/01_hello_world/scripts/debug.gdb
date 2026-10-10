# 先に west debugserver -d <debug build> -r linkserver を起動する。
# SDKのGDBへELFと -batch -x <このファイル> を渡す。
set pagination off
set confirm off
target remote localhost:3333
set mem inaccessible-by-default off
load
monitor reset
break main
continue
list
info locals
print z_main_thread.base.prio
print/x &z_main_thread
x/8wx &z_main_thread
info registers pc sp lr r0
next
step
step
bt
monitor ondisconnect cont
detach
quit
