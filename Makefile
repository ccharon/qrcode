project_name = qrcode

memory_model = -mc
compile_options = -0 $(memory_model) -oh -os -s -zp2 -wcd=880 -zpw -we -xs -i.

# Add the source files for the qrcode library and demo
objs = charconv.obj qrcodegen.obj qrcode.obj

all : clean $(project_name).exe

clean : .symbolic
  @del $(project_name).exe
  @del *.obj
  @del *.map

.cpp.obj :
  wpp $[* $(compile_options)

$(project_name).exe: $(objs)
  wlink system dos option map option eliminate option stack=4096 name $@ file *.obj
