bgcNo = 1
size = 15
x=64
y=60
function setup()
  fillrect(20,20,80,80,7)
end

function _init()--1回だけ
  music(2,64,60,0,3)
  -- music(2,64,130,4,7)--musicNo, Volume0-255,tempo slow140 ~ high60 loopstart,loopend
  fps(30)
    setup()
end

function input()
  -- x = tp(0)
  -- y = tp(1)

  -- if btnp(9) then
  --   -- appmode(appinfo(0), 1)--エディットモードで開く
  --   appmode("/00sample/main.lua", 1)--エディットモードで開く
  -- end

  if btn(1) >= 1 then x = x-3 end
  if btn(2) >= 1 then x = x+3 end
  if btn(3) >= 1 then y = y-3 end
  if btn(4) >= 1 then y = y+3 end
  
end

function _update()--ループします
  input()
end

function _draw()--ループします
  cls(bgcNo)
  fillrect(x,y,size,size,8)
  spr8(1,x,y)
end








  